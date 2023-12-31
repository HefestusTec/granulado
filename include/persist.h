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
#ifndef PERSIST_H
#define PERSIST_H

#include <Preferences.h>

#include "globals.h"
#include "serialCom.h"

namespace PERS {
long int getMicrosStepsByMillimeter();
void setMicrosStepsByMillimeter(long int microsSteps);
long int getMaxMicrosStepsTravel();
void setMaxMicrosStepsTravel(long int microsSteps);

int getLoadCellKnownWeight();
void setLoadCellKnownWeight(int knownWeight);
float getCalibrationFactor();
void setCalibrationFactor(float calibrationFactor);
int getZAxisLengthMillimeters();
void setZAxisLengthMillimeters(int zAxisLengthMillimeters);
double getMaxDeltaLoad();
void setMaxDeltaLoad(double maxDeltaLoad);
double getMaxLoad();
void setMaxLoad(double maxLoad);
double getMaxTravel();
void setMaxTravel(double maxTravel);

void setup();

}  // namespace PERS

#endif