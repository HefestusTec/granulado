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
#include "persist.h"

namespace PERS {
Preferences preferences;

long int getmicrosStepsByMillimeter() {
    preferences.begin("granulado", true);
    long int microsStepsByMillimeter = preferences.getLong("microsStepsByMillimeter", 1000);
    preferences.end();
    return microsStepsByMillimeter;
}

void setmicrosStepsByMillimeter(long int microsteps) {
    preferences.begin("granulado", false);
    preferences.putLong("microsStepsByMillimeter", microsteps);
    preferences.end();
}

long int getmaxMicrosStepsTravel() {
    preferences.begin("granulado", true);
    long int maxMicrosStepsTravel = preferences.getLong("maxMicrosStepsTravel", 1000 * MOTOR_STEPS * MOTOR_MICROS_STEPS);
    preferences.end();
    return maxMicrosStepsTravel;
}

void setmaxMicrosStepsTravel(long int microsteps) {
    preferences.begin("granulado", false);
    preferences.putLong("maxMicrosStepsTravel", microsteps);
    preferences.end();
}

int getLoadCellKnownWeight() {
    preferences.begin("granulado", true);
    long int loadCellKnownWeight = preferences.getLong("loadCellKnownWeight", 1000);
    preferences.end();
    return loadCellKnownWeight;
}

void setLoadCellKnownWeight(int knownWeight) {
    preferences.begin("granulado", false);
    preferences.putInt("loadCellKnownWeight", knownWeight);
    preferences.end();
}

float getCalibrationFactor() {
    preferences.begin("granulado", true);
    float calibrationFactor = preferences.getFloat("calibrationFactor", 1.0);
    preferences.end();
    return calibrationFactor;
}

void setCalibrationFactor(float calibrationFactor) {
    preferences.putFloat("calibrationFactor", calibrationFactor);
    preferences.end();
}

int getZAxisLengthMillimeters() {
    return preferences.getInt("zAxisLenghtMillimeters", 1000);
}

void setZAxisLengthMillimeters(int zAxisLengthMillimeters) {
    preferences.putInt("zAxisLenghtMillimeters", zAxisLengthMillimeters);
    preferences.end();
}
void setup() {
    preferences.begin("granulado", false);
    preferences.end();
}
}  // namespace PERS
