#include <Arduino.h>
#define LED 2
#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0; 
#else 
static const BaseType_t app_cpu = 1; 
#endif

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

void delayLed1(void *parameter){
    for(;;){
        for(uint16_t i = 0; i <= 10; i++){
            digitalWrite(LED, !digitalRead(LED));
            vTaskDelay(100/ portTICK_PERIOD_MS);
        }
        vTaskResume(task_2);
        vTaskSuspend(task_1);
    }
} 
void delayLed2(void *parameter){
    for(;;){
        for(uint16_t i = 0; i <= 10; i++){
            digitalWrite(LED, !digitalRead(LED));
            vTaskDelay(500/ portTICK_PERIOD_MS);
        }
        vTaskResume(task_1);
        vTaskSuspend(task_2);
    }
}
void setup() {
  pinMode(LED, OUTPUT);
  xTaskCreatePinnedToCore(delayLed1, "TAKS 1", 1024, NULL, 2, &task_1, app_cpu);
  xTaskCreatePinnedToCore(delayLed2, "TAKS 2", 1024, NULL, 2, &task_2, app_cpu);
  vTaskSuspend(task_2);
}

void loop() {
    //vTaskDelay(2000/ portTICK_PERIOD_MS);
}