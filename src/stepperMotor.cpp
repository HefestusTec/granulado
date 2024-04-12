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

StepperMotor::StepperMotor() {
}

void StepperMotor::reachedInterrupt(GLOBAL::EndTravelPos topOrBottom) {
    stopMotor();
}

void StepperMotor::moveToTop() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToTop");
    if (!digitalRead(TOP_STOPPER_PIN)) {
        SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToTop: Already at top");
        return;
    }
    moveSteps(-100000);
}

void StepperMotor::moveToBottom() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToBottom");
    if (!digitalRead(BOTTOM_STOPPER_PIN)) {
        SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToBottom: Already at bottom");
        return;
    }
    moveSteps(100000);
}

void StepperMotor::disableMotor() {
    digitalWrite(EN_PLUS, HIGH);
}

void StepperMotor::enableMotor() {
    digitalWrite(EN_PLUS, LOW);
}

void StepperMotor::moveSteps(int steps) {
    isCompress = steps >= 0;
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveSteps: " + String(steps) + " steps");

    if (steps >= 0 ? !digitalRead(BOTTOM_STOPPER_PIN) : !digitalRead(TOP_STOPPER_PIN)) {
        SC::sendMessage(SC::SentMessage::INFO_DEBUG, "Motor already at end");
        return;
    }

    startOfMovementPositionSteps = stepper.getCurrentPositionInSteps();

    enableMotor();
    stepper.setTargetPositionRelativeInSteps(steps);
}

long StepperMotor::getMotorPositionStepsMillimeters() {
    return stepper.getCurrentPositionInSteps() - startOfMovementPositionSteps;
}

long int StepperMotor::stopMotor() {
    stepper.emergencyStop();
    disableMotor();
    SC::sendMessage(SC::SentMessage::STOP_ALERT, "");
    long int currentRelativeSteps = stepper.getCurrentPositionInSteps();
    return currentRelativeSteps;
}

void StepperMotor::setup() {
    // Initialize stepper motor

    microsStepsByMillimeter = PERS::getMicrosStepsByMillimeter();
    zAxisSizeInSteps = PERS::getMaxMicrosStepsTravel();
    zAxisLength = PERS::getZAxisLengthMillimeters();

    stepper.connectToPins(MOTOR_STEP_PIN, MOTOR_DIRECTION_PIN);
    setMotorRPM(rpm);
    stepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    stepper.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    disableMotor();

    stepper.startAsService(0);
}

void StepperMotor::setMotorRPM(int _rpm) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMotorRPM: " + String(_rpm) + " RPM");
    rpm = _rpm;

    stepper.setSpeedInStepsPerSecond((MOTOR_STEPS * rpm) / 60.0);
}

bool StepperMotor::process() {
    bool distanceToTarget = stepper.getDistanceToTargetSigned();

    if (distanceToTarget == 0) {
        disableMotor();
    }
    return distanceToTarget;
}

}  // namespace SM
