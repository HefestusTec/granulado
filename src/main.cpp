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

#include "globalConst.h"
#include "loadSensor.h"
#include "serialCom.h"

TaskHandle_t Task1;

void mainCoreTask(void* pvParameters) {
    Serial.println("main core");
    Serial.println(xPortGetCoreID());

    while (true) {
        // Serial.println("main core");
        delay(1000);
    }
}

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(10);
    }
    uint16_t myArr[SENSOR_BUFFER_SIZE];

    LoadSensor myLoadSensor;

    myLoadSensor.resetSensorReadingsBufferTo(99);

    // Instantiates a SerialCom object
    SC::SerialCom mySerial(myLoadSensor.getDataPackage());

    xTaskCreatePinnedToCore(
        mainCoreTask, /* Task function. */
        "Task1",      /* name of task. */
        10000,        /* Stack size of task */
        NULL,         /* parameter of the task */
        1,            /* priority of the task */
        &Task1,       /* Task handle to keep track of created task */
        0);           /* pin task to core 1 */
}

void loop() {
    // Serial.println("Free memory: " + String(esp_get_free_heap_size()) + " bytes");

    delay(500);
}
