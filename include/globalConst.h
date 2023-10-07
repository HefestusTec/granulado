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

#ifndef GLOBAL_CONSTS_H
#define GLOBAL_CONSTS_H

// Stepper motor pins
#define CP_PLUS 13
// Clockwise pins
#define CW_PLUS 12
// Enable pins
#define EN_PLUS 14

#define TOP_STOPPER_PIN 4
#define BOTTOM_STOPPER_PIN 5
// Stepper motor steps
#define MOTOR_STEPS 256
#define MOTOR_MICROSTEPS 256
#define MOTOR_RPM 120

#define LOADCELL_DOUT_PIN 16
#define LOADCELL_SCK_PIN 17

#define Z_AXIS_LENGTH_MILLIMETER 1000

#endif
