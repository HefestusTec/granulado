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
            moveSteps(-50);
        }
    }
    
}

void StepperMotor::moveToTop() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToTop");
    // if (!digitalRead(TOP_STOPPER_PIN)) {
    //     SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToTop: Already at top");
    //     return;
    // }
    moveSteps(-100000);
}

void StepperMotor::moveToBottom() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToBottom");
    // if (!digitalRead(BOTTOM_STOPPER_PIN)) {
    //     SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveToBottom: Already at bottom");
    //     return;
    // }
    moveSteps(100000);
}

void StepperMotor::disableMotor(){
    digitalWrite(EN_PLUS, HIGH);
}

void StepperMotor::enableMotor(){
    digitalWrite(EN_PLUS, LOW);
}

void StepperMotor::moveSteps(int steps) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "moveSteps: " + String(steps) + " steps");
    
    if (steps >= 0 ? !digitalRead(BOTTOM_STOPPER_PIN) : !digitalRead(TOP_STOPPER_PIN)) {
        SC::sendMessage(SC::SentMessage::INFO_DEBUG, "Motor already at end");
        return;
    }
    
    enableMotor();
    stepper.setTargetPositionRelativeInSteps(steps);
}

int StepperMotor::getMotorPositionStepsMillimeters() {
    return motorPositionSteps / microsStepsByMillimeter;
}

void StepperMotor::calibrate() {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "calibrate");
    STATE::currentState = STATE::StateEnum::CALIBRATING_Z_AXIS;
    calibrationState = CalibratingState::STARTED;
    rpm = MOTOR_DEFAULT_RPM;
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
            disableMotor();
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
    stepper.emergencyStop();
    disableMotor();
    SC::sendMessage(SC::SentMessage::STOP_ALERT, "");
    long int currentRelativeSteps = stepper.getCurrentPositionInSteps();
    motorPositionSteps += currentRelativeSteps - lastRelativePositionSteps;
    lastRelativePositionSteps = currentRelativeSteps;
    return currentRelativeSteps;
}

void StepperMotor::setup()  {
    // Initialize stepper motor
    
    microsStepsByMillimeter = PERS::getMicrosStepsByMillimeter();
    zAxisSizeInSteps = PERS::getMaxMicrosStepsTravel();
    zAxisLength = PERS::getZAxisLengthMillimeters();

    stepper.connectToPins(MOTOR_STEP_PIN, MOTOR_DIRECTION_PIN); 
    setMotorRPM(rpm);              
    stepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    stepper.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    disableMotor();

    /*
    //attach an interrupt to the IO pin of the ermegency stop switch and specify the handler function
    attachInterrupt(digitalPinToInterrupt(EMERGENCY_STOP_PIN), emergencySwitchHandler, RISING);

    //attach an interrupt to the IO pin of the limit switch and specify the handler function
    attachInterrupt(digitalPinToInterrupt(LIMIT_SWITCH_PIN), limitSwitchHandler, CHANGE);
    */

    // tell the ESP_flexystepper in which direction to move when homing is required (only works with a homing / limit switch connected)
    // stepper.setDirectionToHome(-1);
    
    stepper.startAsService(0);
}

void StepperMotor::setMotorRPM(int _rpm) {
    SC::sendMessage(SC::SentMessage::INFO_DEBUG, "setMotorRPM: " + String(_rpm) + " RPM");
    rpm = _rpm;

    stepper.setSpeedInStepsPerSecond((MOTOR_STEPS * rpm) / 60.0 );
}

long StepperMotor::process() {
    long wait_time_micros = stepper.getDistanceToTargetSigned();

    if (wait_time_micros == 0) {
        disableMotor();
    }
    return wait_time_micros;
}

}  // namespace SM
