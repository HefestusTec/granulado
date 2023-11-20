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

#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include "HX711.h"
#include "globals.h"
#include "persist.h"
#include "serialCom.h"

namespace LC {

class LoadCell {
   public:
    LoadCell();

    void tare();
    void calibrateKnownWeight();
    float getInstantaneousReading();
    void setup();
    double getTimeSinceStart();
    float getDeltaLoad();
    void updateReadings();

   private:
    float updateDeltaLoad(float currentLoad);

    HX711 scale;
    float calibrationFactor = 1.0;
    float lastLoad = 0.0f;
    float currentLoad = 0.0f;
    double lastReadingTime = 0.0f;
    float deltaLoad = 0.0f;
};

}  // namespace LC

#endif
