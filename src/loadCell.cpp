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
HX711 scale;

float calibrationFactor = 1.0;
long int readingTime = 0;
long int endReadingTime = 0;
long int readings[SENSOR_BUFFER_SIZE];
long int instantaneousReading = 0;

void calibrate() {
    scale.power_up();
    scale.set_scale();
    scale.tare();  // Reset the scale to 0

    // signal serial communication to wait for calibration
    Serial.println("c");

    // wait for bolinho to answer "k" to start calibration
    SC::setCommStart();
    while (Serial.available() == 0) {
        if (millis() - SC::getCommStart() > SC::getUserTimeout()) {
            Serial.println("t");
            return;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // read serial communication
    String command = Serial.readStringUntil('\n');
    if (command != "k") {
        Serial.println("e");
        return;
    }

    long reading = scale.get_units(LOADCELL_READINGS_PER_SAMPLE);
    long zero_factor = scale.read_average();              // Get a baseline reading
    calibrationFactor = LOADCELL_KNOWN_WEIGHT / reading;  // Divide reading from known weight
    scale.set_scale(calibrationFactor);                   // Adjust to this calibration factor
    scale.power_down();                                   // put the ADC in sleep mode
}

long int getInstaneousReading() {
    return scale.get_units(LOADCELL_READINGS_PER_SAMPLE);
}

void getReadings(bool &stop) {
    scale.power_up();
    while (!stop) {
        Serial.println("r");
        readingTime = millis();
        for (int i = 0; i < SENSOR_BUFFER_SIZE; i++)
            readings[i] = scale.get_units(LOADCELL_READINGS_PER_SAMPLE);
        endReadingTime = millis();
        Serial.println(endReadingTime - readingTime);
        for (int i = 0; i < SENSOR_BUFFER_SIZE; i++)
            Serial.println(readings[i]);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    scale.power_down();
}

void setup() {
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.power_down();  // put the ADC in sleep mode
    calibrationFactor = SC::getCalibrationFactor();
    scale.set_scale(calibrationFactor);  // this value is obtained by calibrating the scale with known weights; see the README for details
}
}  // namespace LC
