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
TaskHandle_t comTaskHandler;

SC::ReceivedCommand currentCommand = SC::ReceivedCommand::NONE;

SM::StepperMotor stepperMotor;
LC::LoadCell loadCell;

bool wasCalibrating = false;
bool lastIsOnTopSwitch = false;
bool lastIsOnBottomSwitch = false;

void topStopInterrupt() {
    stepperMotor.reachedInterrupt(GLOBAL::EndTravelPos::TOP);
    SC::sendMessage(SC::SentMessage::TRIGGERED_TOP_INTERRUPT, "");
}

void bottomStopInterrupt() {
    stepperMotor.reachedInterrupt(GLOBAL::EndTravelPos::BOTTOM);
    SC::sendMessage(SC::SentMessage::TRIGGERED_BOTTOM_INTERRUPT, "");
}

void setup() {
    // Attach interrupt to top sensor
    attachInterrupt(digitalPinToInterrupt(TOP_STOPPER_PIN), topStopInterrupt, FALLING);
    // Attach interrupt to bottom sensor
    attachInterrupt(digitalPinToInterrupt(BOTTOM_STOPPER_PIN), bottomStopInterrupt, FALLING);

    PERS::setup();
    SC::setup();
    loadCell.setup();
    stepperMotor.setup();

    // Creates serial com task
    xTaskCreatePinnedToCore(
        comTask,         /* Function to implement the task */
        "ComTask",       /* Name of the task */
        10000,           /* Stack size in words */
        NULL,            /* Task input parameter */
        0,               /* Priority of the task */
        &comTaskHandler, /* Task handle. */
        0);              /* Core where the task should run */
}

void comTask(void* parameter) {
    while (true) {
        vTaskDelay(10);
        SC::MessageStruct received = SC::getCommand();
        currentCommand = received.command;
        String data = received.data;

        if (currentCommand == SC::ReceivedCommand::NONE) continue;

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
                SC::sendMessage(SC::SentMessage::CURRENT_READING, String(loadCell.getInstaneousReading(), 5));
                break;
            case SC::ReceivedCommand::GET_Z_AXIS_LENGTH:
                SC::sendMessage(SC::SentMessage::Z_AXIS_LENGTH, String(PERS::getZAxisLengthMillimeters()));
                break;
            case SC::ReceivedCommand::MOVE_TO_TOP:
                stepperMotor.moveToTop();
                break;
            case SC::ReceivedCommand::MOVE_X_MM:
                stepperMotor.moveMillimeters(data.toInt());
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
                PERS::setMaxLoad(data.toInt());
                break;
            case SC::ReceivedCommand::SET_MAX_TRAVEL:
                PERS::setMaxTravel(data.toInt());
                break;
            case SC::ReceivedCommand::SET_MAX_DELTA_LOAD:
                PERS::setMaxDeltaLoad(data.toInt());
                break;
            case SC::ReceivedCommand::STOP:
                stepperMotor.stopMotor();
                STATE::currentState = STATE::StateEnum::IDLE;
                break;
            case SC::ReceivedCommand::TARE_LOAD:
                loadCell.tare();
                break;
            case SC::ReceivedCommand::GET_DELTA_LOAD:
                SC::sendMessage(SC::SentMessage::CURRENT_DELTA_LOAD, String(loadCell.getDeltaLoad()));
                break;
            default:
                SC::sendMessage(SC::SentMessage::ERROR, "Invalid command");
                break;
        }

        if (SC::serialBuffer.length()) {
            Serial.print(SC::serialBuffer);
            SC::serialBuffer.clear();
        }
    }
}

void process() {
    /*
    // Check if delta load is bigger than the maximum allowed
    //Serial.print(PERS::getMaxDeltaLoad() + " " + loadCell.getDeltaLoad());
    if (loadCell.getDeltaLoad() > PERS::getMaxDeltaLoad()) {
        stepperMotor.stopMotor();
        STATE::currentState = STATE::StateEnum::IDLE;
        SC::sendMessage(SC::SentMessage::ERROR, "Delta load is bigger than the maximum allowed");
    }

    // Check current load does not exceed the maximum allowed
    if (loadCell.getInstaneousReading() > PERS::getMaxLoad()) {
        stepperMotor.stopMotor();
        STATE::currentState = STATE::StateEnum::IDLE;
        SC::sendMessage(SC::SentMessage::ERROR, "Load is bigger than the maximum allowed");
    }
    */

    // Check if the stepper motor position is bigger than the maximum allowed
    if (stepperMotor.getMotorPositionStepsMillimeters() > stepperMotor.zAxisLength) {
        stepperMotor.stopMotor();
        STATE::currentState = STATE::StateEnum::IDLE;
        SC::sendMessage(SC::SentMessage::ERROR, "Position is bigger than the maximum allowed");
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
