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

#include <ESP_FlexyStepper.h>

#include "globals.h"
#include "persist.h"
#include "serialCom.h"

namespace SM {

class StepperMotor {
   public:
    StepperMotor();
    void moveToTop();
    void moveToBottom();
    void moveSteps(int steps);
    long getMotorPositionStepsMillimeters();
    void setMotorRPM(int rpm);
    long stopMotor();
    void setup();

    // Returns true if is moving
    bool process();

    void reachedInterrupt(GLOBAL::EndTravelPos topOrBottom);

    long microsStepsByMillimeter = 0;
    int zAxisLength = -1;

   private:
    void disableMotor();
    void enableMotor();

    long startOfMovementPositionSteps = 0;
    long zAxisSizeInSteps = 1000 * MOTOR_STEPS * MOTOR_MICROS_STEPS;
    int rpm = 15;

    ESP_FlexyStepper stepper;
};
}  // namespace SM

#endif