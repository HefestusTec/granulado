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
#include <FreeRTOS.h>
#include <Preferences.h>

#include "globalConst.h"
#include "loadCell.h"
#include "stepperMotor.h"

namespace SC {
// Serial communication class
void ping();

long int getMicrostepsByMillimeter();
void setMicrostepsByMillimeter(long int microsteps);
long int getMaxMicrostepsTravel();
void setMaxMicrostepsTravel(long int microsteps);

int getLoadCellKnownWeight();
void setLoadCellKnownWeight(int knownWeight);
float getCalibrationFactor();
void setCalibrationFactor(float calibrationFactor);
int getZAxisLengthMillimeters();
void setZAxisLengthMillimeters(int zAxisLengthMillimeters);

void decodeCommand(bool *stop);

void setup();

}  // namespace SC

#endif