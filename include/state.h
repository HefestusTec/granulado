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
#ifndef STATE_H
#define STATE_H

namespace STATE {

enum class StateEnum {
    NOT_CONNECTED,
    IDLE,
    CALIBRATING_KNOWN_WEIGHT,
    CALIBRATING_Z_AXIS,
    MOVING_Z_AXIS,
    ON_EXPERIMENT,
    RETURNING_TO_HOME
};

extern StateEnum currentState;

}  // namespace STATE

#endif