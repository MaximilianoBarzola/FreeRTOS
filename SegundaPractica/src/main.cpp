#include <Arduino.h>
#define LED 2
#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0; 
#else 
static const BaseType_t app_cpu = 1; 
#endif
static TaskHandle_t tasks_1 = NULL;
static TaskHandle_t tasks_2 = NULL;
volatile uint16_t LED_delay = 10;
void terminal(void *parameter){
    char x = NULL;
    String bafer = "";
    for(;;){
        while(Serial.available()){
            if(Serial.available() > 0){
                x = Serial.read();
            }
            if(x != '\n'){
                bafer += x;
            }
            else if(x = '\n'){
                LED_delay = bafer.toInt();
                x = NULL;
                bafer = "";
            }
        }
    }
    vTaskDelete(NULL);
}
void ledDelay(void *parameter){
    for(;;){
        //Serial.println(LED_delay);
        if(LED_delay < 10){
            digitalWrite(LED, !digitalRead(LED));
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        else{
            digitalWrite(LED, !digitalRead(LED));
            vTaskDelay(LED_delay/ portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(NULL);
}
void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    xTaskCreatePinnedToCore(terminal, "Read Terminal", 1024, NULL, 1, &tasks_1, app_cpu);
    xTaskCreatePinnedToCore(ledDelay, "LED Delay", 1024, NULL, 2, &tasks_2, app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:
}