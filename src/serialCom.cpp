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

uint16_t SC::readValueAt(uint16_t* myArr, uint index) {
    return *(myArr + index);
}

uint16_t* SC::getValueAt(uint16_t* myArr, uint index) {
    return (myArr + index);
}

void SC::serialCoreTask(void* pvParameters) {
    Serial.println("serial core");
    Serial.println(xPortGetCoreID());
    uint16_t* bufferArrayPtr = ((uint16_t*)pvParameters);
    for (uint16_t i = 0; i < SENSOR_BUFFER_SIZE; i++) {
        *SC::getValueAt(bufferArrayPtr, i) = 24;
    }
    while (true) {
        if (Serial.available()) {                           // if there is data comming
            String command = Serial.readStringUntil('\n');  // read string until newline character
            Serial.println(command);
            if (command == "a") {
                Serial.println("1");
            } else if (command == "b") {
                for (uint j = 0; j < SENSOR_BUFFER_SIZE; j++) {
                    Serial.print(readValueAt(bufferArrayPtr, j));

                    Serial.print(",");
                }
                Serial.print("\n");
            }
        }
        vTaskDelay(1);
    }
}

/*
Constructor of the SerialCom class
Receives a pointer to the sensor array buffer
and creates a task attached to core 1.
*/
SC::SerialCom::SerialCom(uint16_t* sensorBuffer) {
    sensBufferPtr = sensorBuffer;

    // create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
    xTaskCreatePinnedToCore(
        serialCoreTask,       /* Task function. */
        "SerialMainTask",     /* name of task. */
        10000,                /* Stack size of task */
        (void*)sensBufferPtr, /* parameter of the task */
        1,                    /* priority of the task */
        &mainTask,            /* Task handle to keep track of created task */
        1);                   /* pin task to core 0 */
}

SC::SerialCom::~SerialCom() {
}