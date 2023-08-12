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

#include <Arduino.h>

#include "BasicStepperDriver.h"  // generic stepper motor driver
#include "globalConst.h"
#include "serialCom.h"

void stepperMotorControllerTask(void* pvParameters) {
    BasicStepperDriver stepper(MOTOR_STEPS, CW_PLUS, CP_PLUS);
    stepper.begin(120, 1);
    int8_t direction = 1;
    long timeForMove = stepper.getTimeForMove(360);

    while (true) {
        Serial.println("Stepper motor rotating");
        Serial.println(direction);
        Serial.println(timeForMove);
        stepper.rotate(direction * 360);
        Serial.println("Stepper motor stopped");
        direction *= -1;

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void serialComTask(void* pvParameters) {
    while (true) {
        if (Serial.available()) {
            char command = Serial.read();
            switch (command) {
                case 'p':
                    SC::ping();
                    break;
                default:
                    Serial.println(command);
                    break;
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(10);
    }

    xTaskCreatePinnedToCore(
        stepperMotorControllerTask,       // Function to run on this core
        "Stepper motor controller task",  // Name of the task
        10000,                            // Stack size in words
        NULL,                             // Task input parameter
        1,                                // Priority of the task
        NULL,                             // Task handle
        0                                 // Core where the task should run
    );

    xTaskCreatePinnedToCore(
        serialComTask,                // Function to run on this core
        "Serial communication task",  // Name of the task
        10000,                        // Stack size in words
        NULL,                         // Task input parameter
        1,                            // Priority of the task
        NULL,                         // Task handle
        1                             // Core where the task should run
    );
}

void loop() {
    delay(1000);
}
