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

#ifndef SERIAL_COM_H
#define SERIAL_COM_H

#include <Arduino.h>

#include "globalConst.h"
#include "protocol.h"

namespace SC {

/*Task that gets executed on the second core*/
void serialCoreTask(void* pvParameters);

/*
Receives a pointer to an array and an index.
Returns the value stored there

WARNING: this function is UNSAFE make sure you aren't
accessing a value out of bounds
*/
uint16_t readValueAt(uint16_t* myArr, uint index);

/*
Receives a pointer to an array and an index.
Returns a POINTER to the value stored there

WARNING: this function is UNSAFE make sure you aren't
accessing a value out of bounds
*/
uint16_t* getValueAt(uint16_t* myArr, uint index);

void sendSensorDataPackage(Protocol::SensorDataPackage* dataPackage);

class SerialCom {
   private:
    /* data */
   public:
    SerialCom(Protocol::SensorDataPackage* sensorDataPtr);
    ~SerialCom();

    TaskHandle_t mainTask;
    Protocol::SensorDataPackage* sensorDataPtr;
};
}  // namespace SC

#endif