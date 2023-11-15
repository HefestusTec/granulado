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

#include "serialCom.h"

namespace SC {

MessageStruct getCommand() {
    MessageStruct result;
    result.command = ReceivedCommand::NONE;
    result.data = "";

    if (Serial.available() == 0) return result;

    // decode utf-8
    String comm = Serial.readStringUntil('\n');

    char cmd = comm.charAt(0);

    switch (cmd) {
        case 'p':
            result.command = ReceivedCommand::PING;
            break;

        case 'm':
            result.command = ReceivedCommand::MOVE_X_MM;
            result.data = comm.substring(1);
            break;

        case 's':
            result.command = ReceivedCommand::STOP;
            break;

        case 't':
            result.command = ReceivedCommand::MOVE_TO_TOP;
            break;

        case 'g':
            result.command = ReceivedCommand::GET_POSITION;
            break;

        case 'r':
            result.command = ReceivedCommand::GET_READINGS;
            break;

        case '@':
            result.command = ReceivedCommand::TARE_LOAD;
            break;

        case 'w':
            result.command = ReceivedCommand::CALIBRATE_KNOWN_WEIGHT;
            break;

        case 'x':
            result.command = ReceivedCommand::SET_KNOWN_WEIGHT;
            result.data = comm.substring(1);
            break;

        case 'y':
            result.command = ReceivedCommand::SET_Z_AXIS_LENGTH;
            result.data = comm.substring(1);
            break;

        case 'j':
            result.command = ReceivedCommand::GET_Z_AXIS_LENGTH;
            break;

        case 'z':
            result.command = ReceivedCommand::CALIBRATE_Z_AXIS;
            break;

        case 'd':
            result.command = ReceivedCommand::GET_DELTA_LOAD;
            break;

        case 'l':
            result.command = ReceivedCommand::SET_MAX_LOAD;
            result.data = comm.substring(1);
            break;

        case 'v':
            result.command = ReceivedCommand::SET_MAX_TRAVEL;
            result.data = comm.substring(1);
            break;

        case 'a':
            result.command = ReceivedCommand::SET_MAX_DELTA_LOAD;
            result.data = comm.substring(1);
            break;

        default:
            result.command = ReceivedCommand::NONE;
            break;
    };
    return result;
}

void sendMessage(SentMessage message, String data) {
    String messageCode = "e";
    switch (message) {
        case SentMessage::PING_RESPONSE:
            messageCode = "p";
            break;
        case SentMessage::CURRENT_READING:
            messageCode = "r";
            break;
        case SentMessage::CURRENT_POSITION:
            messageCode = "g";
            break;
        case SentMessage::Z_AXIS_LENGTH:
            messageCode = "j";
            break;
        case SentMessage::ERROR:
            messageCode = "e";
            break;
        case SentMessage::CURRENT_DELTA_LOAD:
            messageCode = "d";
            break;
        case SentMessage::STOP_ALERT:
            messageCode = "s";
            break;
        case SentMessage::INFO_DEBUG:
            messageCode = "i";
            break;
        default:
            break;
    }
    Serial.println(messageCode + data);
}

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10);
    while (!Serial)
        delay(10);
}

}  // namespace SC
