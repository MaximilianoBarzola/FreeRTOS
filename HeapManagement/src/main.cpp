#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0; 
#else 
static const BaseType_t app_cpu = 1; 
#endif
volatile boolean ready = false;
char* direccion;

void readSerial(void* parameter){
    TickType_t time_init;
    const TickType_t delay = 1000;
    time_init = xTaskGetTickCount();
    for(;;){
        vTaskDelayUntil(&time_init, delay);
        char *a = (char*)pvPortMalloc(1 * sizeof(char));
        a[0] = 'f';
        direccion = a;
        ready = true;
    }
}
void printHeap(void* parameter){
    for(;;){
        if(ready){
            Serial.println(direccion[0]);
            vPortFree(direccion);
            Serial.println(xPortGetFreeHeapSize());
            ready = false;
        }
    }
}
void setup() {
    Serial.begin(112500);
    xTaskCreatePinnedToCore(readSerial, "Task 1", 1024, NULL, 1, NULL, app_cpu);
    xTaskCreatePinnedToCore(printHeap, "Task 2", 1024, NULL, 0, NULL, app_cpu);
    vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}