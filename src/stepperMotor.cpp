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

StepperMotor::StepperMotor() : stepper(MOTOR_STEPS, CW_PLUS, CP_PLUS, EN_PLUS) {
}

void StepperMotor::reachedInterrupt(GLOBAL::EndTravelPos topOrBottom) {
    stopMotor();
    /*
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
            moveMillimeters(-10);
        }
    }
    */
}

void StepperMotor::moveToTop() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToTop");
    if (digitalRead(TOP_STOPPER_PIN)) {
        SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToTop: Already at top");
        return;
    }
    stepper.disable();
    stepper.startMove(INT_MAX);
}

void StepperMotor::moveToBottom() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToBottom");
    if (digitalRead(BOTTOM_STOPPER_PIN)) {
        SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToBottom: Already at bottom");
        return;
    }
    stepper.disable();
    stepper.startMove(INT_MIN);
}

void StepperMotor::moveSteps(int steps) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveSteps: " + String(steps) + " steps");
    /*
    if (distance >= 0 ? digitalRead(BOTTOM_STOPPER_PIN) : digitalRead(TOP_STOPPER_PIN)) {
        SC::sendMessage(SC::SentMessage::INFO_DEBUG, "Motor already at end");
        return;
    }
    */
    stepper.disable();
    stepper.startMove(steps * MOTOR_MICROS_STEPS);
}

int StepperMotor::getMotorPositionStepsMillimeters() {
    return motorPositionSteps / microsStepsByMillimeter;
}

void StepperMotor::calibrate() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "calibrate");
    STATE::currentState = STATE::StateEnum::CALIBRATING_Z_AXIS;
    calibrationState = CalibratingState::STARTED;
}

void StepperMotor::calibrateProcess() {
    switch (calibrationState) {
        case CalibratingState::STARTED:
            calibrationState = CalibratingState::MOVING_TO_TOP;
            moveToTop();
            SC::sendMessage(SC::SentMessage::INFO_DEBUG, "calibrateProcess: Calibration started");
            break;

        case CalibratingState::MOVING_TO_TOP:
            break;

        case CalibratingState::MOVING_TO_BOTTOM:
            break;

        case CalibratingState::FINISHED:
            stepper.enable();
            STATE::currentState = STATE::StateEnum::IDLE;
            microsStepsByMillimeter = zAxisSizeInSteps / PERS::getZAxisLengthMillimeters();
            PERS::setMicrosStepsByMillimeter(microsStepsByMillimeter);
            SC::sendMessage(SC::SentMessage::INFO_DEBUG, "calibrateProcess: Calibration finished");
            break;

        default:
            break;
    }
}

long int StepperMotor::stopMotor() {
    stepper.stop();
    stepper.enable();
    SC::sendMessage(SC::SentMessage::STOP_ALERT, "");
    long int currentRelativeSteps = stepper.getStepsCompleted();
    motorPositionSteps += currentRelativeSteps - lastRelativePositionSteps;
    lastRelativePositionSteps = currentRelativeSteps;
    return currentRelativeSteps;
}

void StepperMotor::setup() {
    // Initialize stepper motor
    stepper.begin(rpm, MOTOR_MICROS_STEPS);

    microsStepsByMillimeter = PERS::getMicrosStepsByMillimeter();
    zAxisSizeInSteps = PERS::getMaxMicrosStepsTravel();
    zAxisLength = PERS::getZAxisLengthMillimeters();

    stepper.disable();
}

void StepperMotor::setMotorRPM(int _rpm) {
    rpm = _rpm;

    stepper.setRPM(rpm);
}

void StepperMotor::process() {
    unsigned wait_time_micros = stepper.nextAction();

    if (wait_time_micros <= 0) {
        stepper.enable();
    }
}

}  // namespace SM
