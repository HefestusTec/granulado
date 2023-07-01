#include <Arduino.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println("Free memory: " + String(esp_get_free_heap_size()) + " bytes");

    delay(500);
}
