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
#include "state.h"

namespace LC {

class LoadCell {
   public:
    LoadCell();

    void tare();
    void calibrateKnownWeight();
    float getInstaneousReading();
    void setup();

   private:
    HX711 scale;
    float calibrationFactor = 1.0;
};

}  // namespace LC

#endif

/*
c:/users/rafae/.platformio/packages/toolchain-xtensa-esp32s3/bin/../lib/gcc/xtensa-esp32s3-elf/8.4.0/../../../../xtensa-esp32s3-elf/bin/ld.exe: .pio/build/esp32-s3-devkitc-1/src/core.cpp.o:


(.literal._ZN4CORE5setupEv+0x18): undefined reference to `CORE::comTask(void*)'

collect2.exe: error: ld returned 1 exit status
*** [.pio\build\esp32-s3-devkitc-1\firmware.elf] Error 1
*/