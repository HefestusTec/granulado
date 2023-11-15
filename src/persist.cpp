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
    preferences.begin("granulado", false);
    if (!preferences.isKey("mSBM")) {
        preferences.putLong("mSBM", 1000);
    }
    long int result = preferences.getLong("mSBM", 1000);
    preferences.end();
    return result;
}

void setMicrosStepsByMillimeter(long int microsSteps) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMicrosStepsByMillimeter");
    preferences.begin("granulado", false);
    preferences.putLong("mSBM", microsSteps);
    preferences.end();
}

long int getMaxMicrosStepsTravel() {
    preferences.begin("granulado", false);
    if (!preferences.isKey("mMST")) {
        preferences.putLong("mMST", 1000 * MOTOR_STEPS * MOTOR_MICROS_STEPS);
    }
    long int result = preferences.getLong("mMST", 1000 * MOTOR_STEPS * MOTOR_MICROS_STEPS);
    preferences.end();
    return result;
}

void setMaxMicrosStepsTravel(long int microsSteps) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxMicrosStepsTravel");
    preferences.begin("granulado", false);
    preferences.putLong("mMST", microsSteps);
    preferences.end();
}

int getLoadCellKnownWeight() {
    preferences.begin("granulado", false);
    if (!preferences.isKey("lCKW")) {
        preferences.putInt("lCKW", 1000);
    }
    long int result = preferences.getLong("lCKW", 1000);
    preferences.end();
    return result;
}

void setLoadCellKnownWeight(int knownWeight) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setLoadCellKnownWeight");
    preferences.begin("granulado", false);
    preferences.putInt("lCKW", knownWeight);
    preferences.end();
}

float getCalibrationFactor() {
    preferences.begin("granulado", false);
    if (!preferences.isKey("cF")) {
        preferences.putFloat("cF", 1.0);
    }
    float result = preferences.getFloat("cF", 1.0);
    preferences.end();
    return result;
}

void setCalibrationFactor(float calibrationFactor) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setCalibrationFactor");
    preferences.begin("granulado", false);
    preferences.putFloat("cF", calibrationFactor);
    preferences.end();
}

int getZAxisLengthMillimeters() {
    preferences.begin("granulado", false);
    if (!preferences.isKey("zALM")) {
        preferences.putInt("zALM", 1000);
    }
    int result = preferences.getInt("zALM", 1000);
    preferences.end();
    return result;
}

void setZAxisLengthMillimeters(int zAxisLengthMillimeters) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setZAxisLengthMillimeters");
    preferences.begin("granulado", false);
    preferences.putInt("zALM", zAxisLengthMillimeters);
    preferences.end();
}

double getMaxDeltaLoad() {
    // Check if exists, it don't, create it
    preferences.begin("granulado", false);
    if (!preferences.isKey("mDL")) {
        preferences.putDouble("mDL", 1000);
    }
    double result = preferences.getDouble("mDL", 1000);
    preferences.end();
    return result;
}

void setMaxDeltaLoad(double maxDeltaLoad) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxDeltaLoad");
    preferences.begin("granulado", false);
    preferences.putDouble("mDL", maxDeltaLoad);
    preferences.end();
}

double getMaxLoad() {
    // Check if exists, it don't, create it
    preferences.begin("granulado", false);
    if (!preferences.isKey("mL")) {
        preferences.putDouble("mL", 1000);
    }
    double result = preferences.getDouble("mL", 1000);
    preferences.end();
    return result;
}

void setMaxLoad(double maxLoad) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxLoad");
    preferences.begin("granulado", false);
    preferences.putDouble("mL", maxLoad);
    preferences.end();
}

double getMaxTravel() {
    // Check if exists, it don't, create it
    preferences.begin("granulado", false);
    if (!preferences.isKey("mT")) {
        preferences.putDouble("mT", 1000);
    }
    double result = preferences.getDouble("mT", 1000);
    preferences.end();
    return result;
}

void setMaxTravel(double maxTravel) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMaxTravel");
    preferences.begin("granulado", false);
    preferences.putDouble("mT", maxTravel);
    preferences.end();
}

void setup() {
    preferences.begin("granulado", false);
    preferences.end();
}
}  // namespace PERS
