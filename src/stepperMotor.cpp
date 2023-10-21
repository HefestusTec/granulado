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

#include "stepperMotor.h"

namespace SM {

StepperMotor::StepperMotor() : stepper(MOTOR_STEPS, CW_PLUS, CP_PLUS) {
}

void StepperMotor::reachedInterrupt(GLOBAL::EndTravelPos topOrBottom) {
    stopMotor();

    if (topOrBottom == GLOBAL::EndTravelPos::TOP) {
        if (calibrationState == CalibratingState::MOVING_TO_TOP) {
            calibrationState = CalibratingState::MOVING_TO_BOTTOM;
            motorPositionSteps = 0;  // Sets the top as 0
            moveToBottom();
        }
    }
    if (topOrBottom == GLOBAL::EndTravelPos::BOTTOM) {
        if (calibrationState == CalibratingState::MOVING_TO_BOTTOM) {
            calibrationState = CalibratingState::FINISHED;
            zAxisSizeInSteps = motorPositionSteps;  // Sets the top as 0
            moveMillimeters(10);
        }
    }
}

void StepperMotor::moveToTop() {
    stepper.startMove(INT_MAX);
}

void StepperMotor::moveToBottom() {
    stepper.startMove(INT_MIN);
}

void StepperMotor::moveMillimeters(int distance) {
    stepper.move(distance);
}

int StepperMotor::getmotorPositionStepsMillimeters() {
    return motorPositionSteps / microsStepsByMillimeter;
}

void StepperMotor::calibrate() {
    STATE::currentState = STATE::StateEnum::CALIBRATING_Z_AXIS;
    calibrationState = CalibratingState::STARTED;
}

void StepperMotor::calibrateProcess() {
    switch (calibrationState) {
        case CalibratingState::STARTED:
            calibrationState = CalibratingState::MOVING_TO_TOP;
            moveToTop();
            Serial.println("Calibration started");
            break;

        case CalibratingState::MOVING_TO_TOP:
            Serial.println("Calibration Moving to top");
            break;

        case CalibratingState::MOVING_TO_BOTTOM:
            Serial.println("Calibration Moving to bottom");
            break;

        case CalibratingState::FINISHED:
            STATE::currentState = STATE::StateEnum::IDLE;
            microsStepsByMillimeter = zAxisSizeInSteps / PERS::getZAxisLengthMillimeters();
            PERS::setmicrosStepsByMillimeter(microsStepsByMillimeter);
            Serial.println("Calibration finished");
            break;

        default:
            break;
    }
}

long int StepperMotor::stopMotor() {
    stepper.stop();
    long int currentRelativeSteps = stepper.getStepsCompleted();
    motorPositionSteps += currentRelativeSteps - lastRelativePositionSteps;
    lastRelativePositionSteps = currentRelativeSteps;
    return currentRelativeSteps;
}

void StepperMotor::setup() {
    // Initialize stepper motor
    stepper.begin(MOTOR_RPM, MOTOR_MICROS_STEPS);

    // Configure stopper pin to read HIGH unless grounded
    pinMode(TOP_STOPPER_PIN, INPUT_PULLUP);
    pinMode(BOTTOM_STOPPER_PIN, INPUT_PULLUP);

    microsStepsByMillimeter = PERS::getmicrosStepsByMillimeter();
    zAxisSizeInSteps = PERS::getmaxMicrosStepsTravel();

    stepper.enable();
}
}  // namespace SM
