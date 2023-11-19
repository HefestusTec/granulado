// Copyright (C) 2023 Hefestus
//
// This file is part of Granulado.
//
// Granulado is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Granulado is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Granulado.  If not, see <http://www.gnu.org/licenses/>.

#include "loadCell.h"

namespace LC {

LoadCell::LoadCell(/* args */) {
}

void LoadCell::tare() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "tare");
    calibrationFactor = PERS::getCalibrationFactor();
    scale.set_scale();
    scale.tare();  // Reset the scale to 0

    scale.set_scale(calibrationFactor);
}

void LoadCell::calibrateKnownWeight() {
    scale.set_scale();
    STATE::currentState = STATE::StateEnum::CALIBRATING_KNOWN_WEIGHT;
    int knownWeight = PERS::getLoadCellKnownWeight();  // Get the known weight from EEPROM
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "calibrateKnownWeight" + String(knownWeight));
    calibrationFactor = scale.get_units(10) / (knownWeight / 1000.f);
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "calibrationFactor" + String(calibrationFactor));
    PERS::setCalibrationFactor(calibrationFactor);  // Save calibration factor to EEPROM
    scale.set_scale(calibrationFactor);             // Adjust to this calibration factor
    STATE::currentState = STATE::StateEnum::IDLE;
}

void LoadCell::updateReadings(){
    currentLoad = scale.get_units(1);  // Get current load in grams
    deltaLoad = updateDeltaLoad(currentLoad);
}


float LoadCell::getInstantaneousReading() {
    return currentLoad;
}

double LoadCell::getTimeSinceStart() {
    // Get the time elapsed since the start of the FreeRTOS system
    int64_t timeSinceStart = esp_timer_get_time();

    // Convert the time to seconds
    return (double)timeSinceStart / 1000000.0;
}

float LoadCell::getDeltaLoad() {
    return deltaLoad;
}

float LoadCell::updateDeltaLoad(float currentLoad) {
    double currentTime = getTimeSinceStart();

    // Get the time elapsed since the last reading
    double timeSinceLastStart = currentTime - lastReadingTime;
    if (timeSinceLastStart == 0)
        timeSinceLastStart = 0.0000001;

    // Calculate the delta load
    float newDeltaLoad = (currentLoad - lastLoad) / timeSinceLastStart;

    // Update the last load and last reading time
    lastLoad = currentLoad;
    lastReadingTime = currentTime;

    return newDeltaLoad;
}

void LoadCell::setup() {
    calibrationFactor = PERS::getCalibrationFactor();
    scale.begin(LOAD_CELL_DOUT_PIN, LOAD_CELL_SCK_PIN);
    scale.power_up();
    scale.set_scale(calibrationFactor);  // this value is obtained by calibrating the scale with known weights; see the README for details
}   
}  // namespace LC
