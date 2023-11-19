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
#include "core.h"

namespace CORE {

SC::ReceivedCommand currentCommand = SC::ReceivedCommand::NONE;

SM::StepperMotor stepperMotor;
LC::LoadCell loadCell;

bool wasCalibrating = false;
bool lastIsOnTopSwitch = false;
bool lastIsOnBottomSwitch = false;

ExperimentLimits expLimits;

void topStopInterrupt() {
    stepperMotor.reachedInterrupt(GLOBAL::EndTravelPos::TOP);
    SC::sendMessage(SC::SentMessage::TRIGGERED_TOP_INTERRUPT, "");
}

void bottomStopInterrupt() {
    stepperMotor.reachedInterrupt(GLOBAL::EndTravelPos::BOTTOM);
    SC::sendMessage(SC::SentMessage::TRIGGERED_BOTTOM_INTERRUPT, "");
}

void setup() {
    // Configure stopper pin to read HIGH unless grounded
    pinMode(TOP_STOPPER_PIN, INPUT);
    pinMode(BOTTOM_STOPPER_PIN, INPUT);
    // Attach interrupt to top sensor
    // attachInterrupt(TOP_STOPPER_PIN, topStopInterrupt, FALLING);
    // // Attach interrupt to bottom sensor
    // attachInterrupt(BOTTOM_STOPPER_PIN, bottomStopInterrupt, FALLING);

    PERS::setup();
    SC::setup();
    loadCell.setup();
    stepperMotor.setup();

    expLimits.maxLoad = PERS::getMaxLoad();
    expLimits.maxDeltaLoad = PERS::getMaxDeltaLoad();
    expLimits.maxTravel = PERS::getMaxTravel();
}

void comTask() {
    SC::MessageStruct received = SC::getCommand();
    currentCommand = received.command;
    String data = received.data;

    if (currentCommand == SC::ReceivedCommand::NONE) return;

    switch (currentCommand) {
        case SC::ReceivedCommand::CALIBRATE_KNOWN_WEIGHT:
            loadCell.calibrateKnownWeight();
            break;
        case SC::ReceivedCommand::CALIBRATE_Z_AXIS:
            stepperMotor.calibrate();
            break;
        case SC::ReceivedCommand::GET_POSITION:
            SC::sendMessage(SC::SentMessage::CURRENT_POSITION, String(stepperMotor.getMotorPositionStepsMillimeters()));
            break;
        case SC::ReceivedCommand::GET_READINGS:
            loadCell.updateReadings();
            SC::sendMessage(SC::SentMessage::CURRENT_READING, String(loadCell.getInstantaneousReading(), 5));
            break;
        case SC::ReceivedCommand::GET_Z_AXIS_LENGTH:
            SC::sendMessage(SC::SentMessage::Z_AXIS_LENGTH, String(PERS::getZAxisLengthMillimeters()));
            break;
        case SC::ReceivedCommand::MOVE_TO_TOP:
            stepperMotor.moveToTop();
            break;
        case SC::ReceivedCommand::MOVE_TO_BOTTOM:
            stepperMotor.moveToBottom();
            break;
        case SC::ReceivedCommand::MOVE_X_MM:
            stepperMotor.moveSteps(data.toInt());
            break;
        case SC::ReceivedCommand::PING:
            SC::sendMessage(SC::SentMessage::PING_RESPONSE, "");
            break;
        case SC::ReceivedCommand::SET_KNOWN_WEIGHT:
            PERS::setLoadCellKnownWeight(data.toInt());
            break;
        case SC::ReceivedCommand::SET_Z_AXIS_LENGTH:
            PERS::setZAxisLengthMillimeters(data.toInt());
            break;
        case SC::ReceivedCommand::SET_MAX_LOAD:
            PERS::setMaxLoad(data.toDouble());
            expLimits.maxLoad = data.toDouble();
            break;
        case SC::ReceivedCommand::SET_MAX_TRAVEL:
            PERS::setMaxTravel(data.toInt());
            expLimits.maxTravel = data.toInt();
            break;
        case SC::ReceivedCommand::SET_MAX_DELTA_LOAD:
            PERS::setMaxDeltaLoad(data.toDouble());
            expLimits.maxDeltaLoad = data.toDouble();
            break;
        case SC::ReceivedCommand::STOP:
            SC::sendMessage(SC::SentMessage::INFO_DEBUG, "Received Stop motor");
            stepperMotor.stopMotor();

            STATE::currentState = STATE::StateEnum::IDLE;
            break;
        case SC::ReceivedCommand::TARE_LOAD:
            loadCell.tare();
            break;
        case SC::ReceivedCommand::GET_DELTA_LOAD:
            SC::sendMessage(SC::SentMessage::CURRENT_DELTA_LOAD, String(loadCell.getDeltaLoad()));
            break;
        case SC::ReceivedCommand::SET_MOTOR_RPM:
            stepperMotor.setMotorRPM(data.toInt());
            break;
        default:
            SC::sendMessage(SC::SentMessage::ERROR, "Invalid command");
            break;
    }
    SC::sendSerialBuffer();
}

void checkStopParams(){
    if(loadCell.getInstantaneousReading() <= expLimits.maxLoad){
        return;
    }
    if(loadCell.getDeltaLoad() <= expLimits.maxDeltaLoad ){
        return;
    }
    if(stepperMotor.getMotorPositionStepsMillimeters() <= expLimits.maxTravel ){
        return;
    }
    stepperMotor.stopMotor();
}


void process() {
    comTask();
    bool isOnTopSwitch = digitalRead(TOP_STOPPER_PIN);
    if (isOnTopSwitch != lastIsOnTopSwitch) {
        topStopInterrupt();
        lastIsOnTopSwitch = isOnTopSwitch;
    }

    bool isOnBottomSwitch = digitalRead(BOTTOM_STOPPER_PIN);
    if (isOnBottomSwitch != lastIsOnBottomSwitch) {
        bottomStopInterrupt();
        lastIsOnBottomSwitch = isOnBottomSwitch;
    }

    if(stepperMotor.process()){
        /*
            Reset to global max after ending an experiment
        */

        checkStopParams();
    }

    switch (STATE::currentState) {
        case STATE::StateEnum::CALIBRATING_Z_AXIS:
            stepperMotor.calibrateProcess();
            break;
        default:
            break;
    }
}

}  // namespace CORE
