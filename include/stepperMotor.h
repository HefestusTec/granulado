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

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <BasicStepperDriver.h>

#include "globals.h"
#include "persist.h"
#include "serialCom.h"
#include "state.h"

namespace SM {
enum class CalibratingState {
    STARTED,
    MOVING_TO_TOP,
    MOVING_TO_BOTTOM,
    FINISHED
};

class StepperMotor {
   public:
    StepperMotor();
    void moveToTop();
    void moveToBottom();
    void moveMillimeters(int distance);
    int getMotorPositionStepsMillimeters();
    void calibrate();
    void calibrateProcess();
    void checkStop();
    long stopMotor();
    void setup();

    void reachedInterrupt(GLOBAL::EndTravelPos topOrBottom);

    long int motorPositionSteps = 0;
    long int microsStepsByMillimeter = 0;
    int zAxisLength = -1;

    CalibratingState calibrationState = CalibratingState::FINISHED;

   private:
    long int lastRelativePositionSteps = 0;
    long int zAxisSizeInSteps = 1000 * MOTOR_STEPS * MOTOR_MICROS_STEPS;

    BasicStepperDriver stepper;
};
}  // namespace SM

#endif