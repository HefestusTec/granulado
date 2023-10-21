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
String stringToSend;  // This isn't a queue because coms need to be synchronous between send and receive
SemaphoreHandle_t comMutex;
SC::ReceivedCommand currentCommand = SC::ReceivedCommand::NONE;

SM::StepperMotor stepperMotor;

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

    LC::setup();
    SC::setup();
    PERS::setup();
    stepperMotor.setup();
    comMutex = xSemaphoreCreateMutex();

    if (comMutex == NULL) {
        Serial.println("Error during Mutex creation");
        while (true) {
        }
    }

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

void comTask() {
    while (true) {
        vTaskDelay(10);
        SC::ReceivedStruct received = SC::getCommand();
        currentCommand = received.command;

        if (currentCommand == SC::ReceivedCommand::NONE) continue;

        switch (currentCommand) {
            case SC::ReceivedCommand::CALIBRATE_KNOWN_WEIGHT:
                LC::calibrateKnownWeight();
                break;
            case SC::ReceivedCommand::CALIBRATE_Z_AXIS:
                stepperMotor.calibrate();
                break;
            case SC::ReceivedCommand::GET_POSITION:
                break;
            case SC::ReceivedCommand::GET_READINGS:
                break;
            case SC::ReceivedCommand::GET_Z_AXIS_LENGTH:
                break;
            case SC::ReceivedCommand::MOVE_TO_TOP:
                stepperMotor.moveToTop();
                break;
            case SC::ReceivedCommand::MOVE_X_MM:
                break;
            case SC::ReceivedCommand::PING:
                SC::ping();
                break;
            case SC::ReceivedCommand::SET_KNOWN_WEIGHT:
                break;
            case SC::ReceivedCommand::SET_Z_AXIS_LENGTH:
                break;
            case SC::ReceivedCommand::STOP:
                break;
            case SC::ReceivedCommand::TARE_LOAD:
                break;
            default:
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
