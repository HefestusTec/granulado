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

#ifndef SERIAL_COM_H
#define SERIAL_COM_H

#include <Arduino.h>
#include <FreeRTOS.h>

#include "globals.h"

namespace SC {

enum class ReceivedCommand {
    CALIBRATE_KNOWN_WEIGHT = 'w',
    CALIBRATE_Z_AXIS = 'z',
    GET_POSITION = 'g',
    GET_READINGS = 'r',
    GET_Z_AXIS_LENGTH = 'j',
    MOVE_TO_TOP = 't',
    MOVE_X_MM = 'm',
    NONE = '-',
    PING = 'p',
    SET_KNOWN_WEIGHT = 'x',
    SET_Z_AXIS_LENGTH = 'y',
    STOP = 's',
    TARE_LOAD = '@',
    SET_MAX_LOAD = 'l',  // MUST implement
    SET_MAX_TRAVEL = 'v'
};

enum class SentMessage {
    PING_RESPONSE = 'p',
    CURRENT_READING = 'r',
    CURRENT_POSITION = 'g',
    Z_AXIS_LENGTH = 'j',
    ERROR = 'e',
    TRIGGERED_BOTTOM_INTERRUPT = 'b',
    TRIGGERED_TOP_INTERRUPT = 't',
    //
    DELTA_LOAD = 'd',
    S

};

struct MessageStruct {
    ReceivedCommand command;
    String data;
};

String readSerialMessage();
MessageStruct getCommand();

void sendMessage(SentMessage message, String data);

void setup();

}  // namespace SC

#endif