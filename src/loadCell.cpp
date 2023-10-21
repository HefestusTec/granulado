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
    scale.set_scale();
    scale.tare();  // Reset the scale to 0
}

void LoadCell::calibrateKnownWeight() {
    STATE::currentState = STATE::StateEnum::CALIBRATING_KNOWN_WEIGHT;
    long reading = scale.get_units(80);
    calibrationFactor = PERS::getLoadCellKnownWeight() / reading;  // Divide reading from known weight
    scale.set_scale(calibrationFactor);                            // Adjust to this calibration factor
    PERS::setCalibrationFactor(calibrationFactor);                 // Save calibration factor to EEPROM
    STATE::currentState = STATE::StateEnum::IDLE;
}

float LoadCell::getInstaneousReading() {
    float weight = (float)scale.read() * calibrationFactor;
    return weight;
}

void LoadCell::setup() {
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    calibrationFactor = PERS::getCalibrationFactor();
    scale.power_up();
    scale.set_scale(calibrationFactor);  // this value is obtained by calibrating the scale with known weights; see the README for details
}
}  // namespace LC
