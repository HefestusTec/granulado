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

#include "controller.h"

namespace CTRL {
bool stop;

TaskHandle_t loadCellTaskHandle;
TaskHandle_t stepperMotorTaskHandle;

void loadLossControl() {
    stop = false;
    // start RTOS task on core 0
    xTaskCreatePinnedToCore(
        SM::moveToBottom,                 // task function
        "stepperMotor_moveToBottomTask",  // task name
        10000,                            // stack size
        &stop,                            // task parameters
        1,                                // priority
        &stepperMotorTaskHandle,          // task handle
        0                                 // core
    );

    // start RTOS task on core 1
    xTaskCreatePinnedToCore(
        LC::getReadings,         // task function
        "loadCellReadingsTask",  // task name
        10000,                   // stack size
        &stop,                   // task parameters
        1,                       // priority
        &loadCellTaskHandle,     // task handle
        1                        // core
    );


}

void getInstantaneousReading() {

    Serial.println(LC::getInstaneousReading());
    Serial.println(SM::getMotorPosition());
}

void setup() {
    SC::setup();
    LC::setup();
    SM::setup();
}

void loop() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
}  // namespace CTRL
