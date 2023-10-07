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
bool stop;

Preferences preferences;

void ping() {
    Serial.println("p");
}

long int getMicrostepsByMillimeter() {
    preferences.begin("granulado", true);
    long int microstepsByMillimeter = preferences.getLong("microstepsByMillimeter", 1000);
    preferences.end();
    return microstepsByMillimeter;
}

void setMicrostepsByMillimeter(long int microsteps) {
    preferences.begin("granulado", false);
    preferences.putLong("microstepsByMillimeter", microsteps);
    preferences.end();
}

long int getMaxMicrostepsTravel() {
    preferences.begin("granulado", true);
    long int maxMicrostepsTravel = preferences.getLong("maxMicrostepsTravel", 1000 * MOTOR_STEPS * MOTOR_MICROSTEPS);
    preferences.end();
    return maxMicrostepsTravel;
}

void setMaxMicrostepsTravel(long int microsteps) {
    preferences.begin("granulado", false);
    preferences.putLong("maxMicrostepsTravel", microsteps);
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

void decodeCommand(bool *stop) {
    while (Serial.available() == 0)
        vTaskDelay(1);
    String comm = Serial.readStringUntil('\n');
    char cmd = comm.charAt(0);
    switch (cmd) {
        case 'p':
            SC::ping();
            break;

        case 'm':
            int millimeters = comm.substring(1).toInt();
            char *moveParameters = (char *)malloc(sizeof(int) + sizeof(bool *));
            memcpy(moveParameters, &millimeters, sizeof(int));
            memcpy(moveParameters + sizeof(int), &stop, sizeof(bool *));

            SM::moveMillimeters(moveParameters);
            break;

        case 's':
            stop = true;
            break;

        case 't':
            SM::moveToTop(&stop);
            break;

        case 'g':
            SM::getMotorPositionMillimeters();
            break;

        case 'r':
            LC::getInstaneousReading();
            break;

        case '@':
            LC::tare();
            break;

        case 'w':
            LC::calibrateKnownWeight();
            break;

        case 'x':
            float weight = comm.substring(1).toFloat();
            SC::setLoadCellKnownWeight(weight);
            break;

        case 'y':
            float zAxisLengthMillimeters = comm.substring(1).toFloat();
            SC::setZAxisLengthMillimeters(zAxisLengthMillimeters);
            break;

        case 'j':
            int millimiters = SC::getZAxisLengthMillimeters();
            // String zAxisLengthString = String(millimiters);
            // Serial.println("j" + zAxisLengthString);
            break;
        case 'z':
            void *calibrationParameters = (void *)malloc(sizeof(int) + sizeof(bool *));
            SM::calibrate(calibrationParameters);
            break;
        default:
            Serial.println("edefault_value");
            break;
    };
    vTaskDelay(1);
}

void setup() {
    Serial.begin(115200);
    preferences.begin("granulado", false);
    preferences.end();
}

}  // namespace SC
