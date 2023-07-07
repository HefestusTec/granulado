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

#ifndef LOAD_SENSORS_H
#define LOAD_SENSORS_H

#include <Arduino.h>

#include "globalConst.h"
#include "protocol.h"

class LoadSensor {
   public:
    LoadSensor();
    uint16_t* getSensorReadingsBuffer();
    void resetSensorReadingsBufferTo(uint16_t defaultValue);
    Protocol::SensorDataPackage* getDataPackage();

   private:
    uint16_t sensorReadingsBuffer[SENSOR_BUFFER_SIZE];
    bool hasOverflown = false;

    // Stores the time when the readings began
    uint32_t readingStartTime;

    // Stores the time when the readings ended
    uint32_t readingEndTime;

    Protocol::SensorDataPackage dataPackage;
};

#endif