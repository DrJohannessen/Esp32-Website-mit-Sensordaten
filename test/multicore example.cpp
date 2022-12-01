#include <Arduino.h>

TaskHandle_t Core0TaskHnd;
TaskHandle_t Core1TaskHnd;

void CoreTask0(void *parameter)
{
    for (;;)
    {
        Serial.print("CoreTask0 runs on Core: ");
        Serial.println(xPortGetCoreID());
        yield();
        delay(600);
    }
}

void CoreTask1(void *parameter)
{
    for (;;)
    {
        Serial.print("CoreTask1 runs on Core: ");
        Serial.println(xPortGetCoreID());
        delay(700);
    }
}

void setup()
{
    Serial.begin(9600);
    xTaskCreatePinnedToCore(CoreTask0, "CPU_0", 1000, NULL, 1, &Core0TaskHnd, 0);
    xTaskCreatePinnedToCore(CoreTask1, "CPU_1", 1000, NULL, 1, &Core0TaskHnd, 1);
}

void loop()
{
    Serial.print("Application CPU is on core:");
    Serial.println(xPortGetCoreID());
    delay(500);
}