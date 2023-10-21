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

void topStopInterrupt() {
    stepperMotor.reachedInterrupt(GLOBAL::EndTravelPos::TOP);
}

void bottomStopInterrupt() {
    stepperMotor.reachedInterrupt(GLOBAL::EndTravelPos::BOTTOM);
}

void setup() {
    // Attach interrupt to top sensor
    attachInterrupt(digitalPinToInterrupt(TOP_STOPPER_PIN), topStopInterrupt, RISING);
    // Attach interrupt to bottom sensor
    attachInterrupt(digitalPinToInterrupt(BOTTOM_STOPPER_PIN), bottomStopInterrupt, RISING);

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
                SC::sendMessage(SC::SentMessage::CURRENT_POSITION, String(stepperMotor.getmotorPositionStepsMillimeters()));
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
            case SC::ReceivedCommand::STOP:
                stepperMotor.stopMotor();
                STATE::currentState = STATE::StateEnum::IDLE;
                break;
            case SC::ReceivedCommand::TARE_LOAD:
                loadCell.tare();
                break;
            default:
                SC::sendMessage(SC::SentMessage::ERROR, "Invalid command");
                break;
        }
    }
}

void process() {
    if (STATE::currentState == STATE::StateEnum::NOT_CONNECTED) {
        return;
    }
    switch (STATE::currentState) {
        case STATE::StateEnum::NOT_CONNECTED:
            /* code */
            break;
        case STATE::StateEnum::MOVING_Z_AXIS:
            break;
        case STATE::StateEnum::CALIBRATING_Z_AXIS:
            stepperMotor.calibrateProcess();
            break;
        default:
            _tryConnect();
            break;
    }
}

void _tryConnect() {
}

}  // namespace CORE
