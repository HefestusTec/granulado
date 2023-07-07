#include <Arduino.h>

#define RND_ARRAY_SIZE 1000

void serialCoreTask(void* pvParameters) {
    Serial.println("serial core");
    Serial.println(xPortGetCoreID());
    uint32_t myArr[RND_ARRAY_SIZE];
    while (true) {
        for (int i = 0; i < RND_ARRAY_SIZE; i++) {
            myArr[i] = esp_random();
        }
        if (Serial.available()) {                           // if there is data comming
            String command = Serial.readStringUntil('\n');  // read string until newline character
            Serial.println(command);
            if (command == "a") {
                for (int j = 0; j < RND_ARRAY_SIZE; j++) {
                    Serial.print(myArr[j]);
                    Serial.print(",");
                }
                Serial.print("\n");
            }
        }
        vTaskDelay(1);
    }
}

void mainCoreTask(void* pvParameters) {
    Serial.println("main core");
    Serial.println(xPortGetCoreID());

    while (true) {
        // Serial.println("main core");
        delay(1000);
    }
}

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(10);
    }
    // create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
    xTaskCreatePinnedToCore(
        serialCoreTask, /* Task function. */
        "Task1",        /* name of task. */
        100000,          /* Stack size of task */
        NULL,           /* parameter of the task */
        1,              /* priority of the task */
        &Task1,         /* Task handle to keep track of created task */
        0);             /* pin task to core 0 */
    delay(3);

    // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
    xTaskCreatePinnedToCore(
        mainCoreTask, /* Task function. */
        "Task2",      /* name of task. */
        10000,        /* Stack size of task */
        NULL,         /* parameter of the task */
        1,            /* priority of the task */
        &Task2,       /* Task handle to keep track of created task */
        1);           /* pin task to core 1 */
}

void loop() {
    // Serial.println("Free memory: " + String(esp_get_free_heap_size()) + " bytes");

    delay(500);
}
