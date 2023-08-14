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

#include "stepperMotor.h"

namespace SM {
BasicStepperDriver stepper(MOTOR_STEPS, CW_PLUS, CP_PLUS);
long int motorPosition = 0;
long int lastRelativePosition = 0;
long int currentRelativePosition = 0;
long int microstepsByMillimeter = 0;
long maxMicrostepsTravel = 1000 * MOTOR_STEPS * MOTOR_MICROSTEPS;

void moveToTop(void *pvParameters) {
    if (pvParameters == NULL) return;
    // create a reference to the stop flag
    bool &stop = *reinterpret_cast<bool *>(pvParameters);
    stop = false;
    // Move to top stopper
    stepper.startMove(maxMicrostepsTravel);
    while (digitalRead(TOP_STOPPER_PIN) == HIGH && !stop) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void moveToBottom(void *pvParameters) {
    if (pvParameters == NULL) return;
    bool &stop = *reinterpret_cast<bool *>(pvParameters);
    stop = false;
    // Move to bottom stopper
    stepper.startMove(-maxMicrostepsTravel);
    while (digitalRead(BOTTOM_STOPPER_PIN) == HIGH && !stop) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void moveMillimeters(void *pvParameters) {
    if (pvParameters == NULL) return;
    long int millimeters;
    memcpy(&millimeters, pvParameters, sizeof(millimeters));
    // create a reference to the stop flag
    bool &stop = *reinterpret_cast<bool *>(pvParameters + sizeof(millimeters));

    long int microsteps = millimeters * microstepsByMillimeter;
    long int endPosition = motorPosition + microsteps;

    stepper.startMove(microsteps);
    stop = false;

    while (motorPosition < endPosition && !stop) {
        currentRelativePosition = stepper.getStepsCompleted();
        motorPosition += currentRelativePosition - lastRelativePosition;
        lastRelativePosition = currentRelativePosition;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void getMotorPositionMillimeters() {
    String mp = String(motorPosition / microstepsByMillimeter);
    Serial.println("g" + mp);
}

void calibrate(void *pvParameters) {
    if (pvParameters == NULL) return;
    // create a reference to the stop flag
    bool &stop = *reinterpret_cast<bool *>(pvParameters);
    moveToTop(pvParameters);
    if (stop) return;
    moveToBottom(pvParameters);
    if (stop) return;

    maxMicrostepsTravel = stepper.getStepsCompleted();
    microstepsByMillimeter = maxMicrostepsTravel / SC::getZAxisLengthMillimeters();
    SC::setMicrostepsByMillimeter(microstepsByMillimeter);
    if (stop) return;

    moveToTop(pvParameters);
    if (stop) return;
    motorPosition = 0;
}

void checkStop(void *pvParameters) {
    if (pvParameters == NULL) return;
    bool &stop = *reinterpret_cast<bool *>(pvParameters);
    while (true) {
        if (stop)
            stopperISR();
    }
}

void stopperISR() {
    stepper.stop();
    currentRelativePosition = stepper.getStepsCompleted();
    motorPosition += currentRelativePosition - lastRelativePosition;
    lastRelativePosition = currentRelativePosition;
}

void setup() {
    // Initialize stepper motor
    stepper.begin(MOTOR_RPM, MOTOR_MICROSTEPS);
    
    // Attach interrupt to top sensor
    attachInterrupt(digitalPinToInterrupt(TOP_STOPPER_PIN), stopperISR, RISING);
    // Attach interrupt to bottom sensor
    attachInterrupt(digitalPinToInterrupt(BOTTOM_STOPPER_PIN), stopperISR, RISING);

    // Configure stopper pin to read HIGH unless grounded
    pinMode(TOP_STOPPER_PIN, INPUT_PULLUP);
    pinMode(BOTTOM_STOPPER_PIN, INPUT_PULLUP);

    microstepsByMillimeter = SC::getMicrostepsByMillimeter();
    maxMicrostepsTravel = SC::getMaxMicrostepsTravel();

    stepper.enable();
}

}  // namespace SM
