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

#include "loadSensor.h"

LoadSensor::LoadSensor() {
    resetSensorReadingsBufferTo(0);
    dataPackage.dataPtr = getSensorReadingsBuffer();
    dataPackage.endTime = &readingEndTime;
    dataPackage.startTime = &readingStartTime;
    dataPackage.hasOverflown = &hasOverflown;
}

uint16_t* LoadSensor::getSensorReadingsBuffer() {
    return &sensorReadingsBuffer[0];
}
void LoadSensor::resetSensorReadingsBufferTo(uint16_t defaultValue) {
    // memset(sensorReadingsBuffer, defaultValue, SENSOR_BUFFER_SIZE * sizeof(*sensorReadingsBuffer));
}

Protocol::SensorDataPackage* LoadSensor::getDataPackage() {
    return &dataPackage;
}
