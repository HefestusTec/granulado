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

#include "serialCom.h"

namespace SC {
long int commStart;

Preferences preferences;

void ping() {
    Serial.println("p");
}

void setCommStart() {
    commStart = millis();
}

long int getCommStart() {
    return commStart;
}

long int getCommTimeout() {
    return preferences.getLong("commTimeout", 1000);
    ;
}

void setCommTimeout(long int timeout) {
    preferences.putLong("commTimeout", timeout);
    preferences.end();
}

long int getUserTimeout() {
    return preferences.getLong("userTimeout", 30000);
    ;
}

void setUserTimeout(long int timeout) {
    preferences.putLong("userTimeout", timeout);
    preferences.end();
}

long int getMicrostepsByMillimeter() {
    preferences.getLong("microstepsByMillimeter", 1000);
}

void setMicrostepsByMillimeter(long int microsteps) {
    preferences.putLong("microstepsByMillimeter", microsteps);
    preferences.end();
}

long int getMaxMicrostepsTravel() {
    preferences.getLong("maxMicrostepsTravel", 1000 * MOTOR_STEPS * MOTOR_MICROSTEPS);
}

void setMaxMicrostepsTravel(long int microsteps) {
    preferences.putLong("maxMicrostepsTravel", microsteps);
    preferences.end();
}

float getCalibrationFactor() {
    return preferences.getFloat("calibrationFactor", 1.0);
}

void setCalibrationFactor(float calibrationFactor) {
    preferences.putFloat("calibrationFactor", calibrationFactor);
    preferences.end();
}

void setup() {
    Serial.begin(115200);
    preferences.begin("granulado", false);
    preferences.getLong("commTimeout", 1000);
    preferences.getLong("userTimeout", 30000);
    preferences.end();
}

}  // namespace SC
