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
    scale.set_scale();
    scale.tare();  // Reset the scale to 0
}

void LoadCell::calibrateKnownWeight() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "calibrateKnownWeight");
    STATE::currentState = STATE::StateEnum::CALIBRATING_KNOWN_WEIGHT;
    long reading = scale.get_units(80);
    calibrationFactor = PERS::getLoadCellKnownWeight() / reading;  // Divide reading from known weight
    scale.set_scale(calibrationFactor);                            // Adjust to this calibration factor
    PERS::setCalibrationFactor(calibrationFactor);                 // Save calibration factor to EEPROM
    STATE::currentState = STATE::StateEnum::IDLE;
}

float LoadCell::getInstaneousReading() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "getInstaneousReading");
    float weight = (float)scale.read() * calibrationFactor;
    return weight;
}

double LoadCell::getTimeSinceStart() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "getTimeSinceStart");
    // Get the time elapsed since the start of the FreeRTOS system
    int64_t timeSinceStart = esp_timer_get_time();

    // Convert the time to seconds
    return (double)timeSinceStart / 1000000.0;
}

int LoadCell::getDeltaLoad() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "getDeltaLoad");
    // Get the current load
    float currentLoad = getInstaneousReading();

    double currentTime = getTimeSinceStart();

    // Get the time elapsed since the last reading
    double timeSinceLastStart = currentTime - lastReadingTime;
    if (timeSinceLastStart == 0)
        timeSinceLastStart = 0.0000001;

    // Calculate the delta load
    int deltaLoad = (currentLoad - lastLoad) / timeSinceLastStart;

    // Update the last load and last reading time
    lastLoad = currentLoad;
    lastReadingTime = currentTime;

    return deltaLoad;
}

void LoadCell::setup() {
    scale.begin(LOAD_CELL_DOUT_PIN, LOAD_CELL_SCK_PIN);
    calibrationFactor = PERS::getCalibrationFactor();
    scale.power_up();
    scale.set_scale(calibrationFactor);  // this value is obtained by calibrating the scale with known weights; see the README for details
}
}  // namespace LC
