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

long int getMicrosStepsByMillimeter() {
    preferences.begin("granulado", true);
    long int microsStepsByMillimeter = preferences.getLong("microsStepsByMillimeter", 1000);
    preferences.end();
    return microsStepsByMillimeter;
}

void setMicrosStepsByMillimeter(long int microsSteps) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMicrosStepsByMillimeter");
    preferences.begin("granulado", false);
    preferences.putLong("microsStepsByMillimeter", microsSteps);
    preferences.end();
}

long int getMaxMicrosStepsTravel() {
    preferences.begin("granulado", true);
    long int maxMicrosStepsTravel = preferences.getLong("maxMicrosStepsTravel", 1000 * MOTOR_STEPS * MOTOR_MICROS_STEPS);
    preferences.end();
    return maxMicrosStepsTravel;
}

void setMaxMicrosStepsTravel(long int microsSteps) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxMicrosStepsTravel");
    preferences.begin("granulado", false);
    preferences.putLong("maxMicrosStepsTravel", microsSteps);
    preferences.end();
}

int getLoadCellKnownWeight() {
    preferences.begin("granulado", true);
    long int loadCellKnownWeight = preferences.getLong("loadCellKnownWeight", 1000);
    preferences.end();
    return loadCellKnownWeight;
}

void setLoadCellKnownWeight(int knownWeight) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setLoadCellKnownWeight");
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
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setCalibrationFactor");
    preferences.putFloat("calibrationFactor", calibrationFactor);
    preferences.end();
}

int getZAxisLengthMillimeters() {
    return preferences.getInt("zAxisLengthMillimeters", 1000);
}

void setZAxisLengthMillimeters(int zAxisLengthMillimeters) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setZAxisLengthMillimeters");
    preferences.putInt("zAxisLengthMillimeters", zAxisLengthMillimeters);
    preferences.end();
}

double getMaxDeltaLoad() {
    return preferences.getDouble("maxDeltaLoad", 1000);
}

void setMaxDeltaLoad(double maxDeltaLoad) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxDeltaLoad");
    preferences.putDouble("maxDeltaLoad", maxDeltaLoad);
    preferences.end();
}

double getMaxLoad() {
    return preferences.getDouble("maxLoad", 1000);
}

void setMaxLoad(double maxLoad) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxLoad");
    preferences.putDouble("maxLoad", maxLoad);
    preferences.end();
}

double getMaxTravel() {
    return preferences.getDouble("maxTravel", 1000);
}

void setMaxTravel(double maxTravel) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxTravel");
    preferences.putDouble("maxTravel", maxTravel);
    preferences.end();
}

void setup() {
    preferences.begin("granulado", false);
    preferences.end();
}
}  // namespace PERS
