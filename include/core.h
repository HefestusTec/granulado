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

#ifndef CORE_H
#define CORE_H

#include "globals.h"
#include "loadCell.h"
#include "persist.h"
#include "serialCom.h"
#include "stepperMotor.h"

namespace CORE {

struct ExperimentLimits{
    double maxLoad;
    int maxTravel;
    double maxDeltaLoad;
};

void setup();

void process();  // Runs on the main core

void comTask();  // Is setup to run on the second core

void topStopInterrupt();
void bottomStopInterrupt();

void checkStopParams();

extern ExperimentLimits expLimits;

}  // namespace CORE

#endif