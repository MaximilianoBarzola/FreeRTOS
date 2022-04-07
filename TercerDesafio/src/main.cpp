#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0; 
#else 
static const BaseType_t app_cpu = 1; 
#endif
#define LED1 27
#define LED2 25
#define LED3 2
#define  BUTTON1 18
#define BUTTON2 19
void ledOne(void *parameter) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1000;
    xLastWakeTime = xTaskGetTickCount();
    bool led = false;
    for(;;){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        //digitalWrite(LED1, !digitalRead(LED1));
        led = !led;
        Serial.print("LED 1:");
        Serial.println(led);
    }
    vTaskDelete(NULL);
}
void ledTwo(void *parameter){
    bool led = false;
    uint32_t last_time = millis();
    for(;;){
        //digitalWrite(LED2, digitalRead(BUTTON1));
        if(led != digitalRead(BUTTON1)){
            if(last_time + 20 < millis()){
                Serial.print("LED 2:");
                Serial.println(!led);
                led = !led;
                last_time = millis();
            }
        }
    }
    vTaskDelete(NULL);
}
void ledTree(void *parameter){
    //uint32_t real_time = millis();
    uint32_t last_time = millis();
    int16_t pulse = 0;  
    for(;;){
        if(digitalRead(BUTTON2)){
            last_time = millis();
            while (last_time + 1000 > millis()){
                if(digitalRead(BUTTON2)){
                    if(last_time + 100 < millis()){
                        pulse++;
                        last_time = millis();
                    }
                }
            }
            for(uint16_t i = 0; i <= pulse; i++){
                digitalWrite(LED3, !digitalRead(LED3));
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
            digitalWrite(LED3, 0);
            pulse = 0;
        }
    }
    vTaskDelete(NULL);
}
void setup() {

    //pinMode(LED1, OUTPUT);
    //pinMode(LED2, OUTPUT);
    Serial.begin(112500);
    pinMode(LED3, OUTPUT);
    pinMode(BUTTON1, INPUT_PULLDOWN);
    pinMode(BUTTON2, INPUT_PULLDOWN);
    xTaskCreatePinnedToCore(ledOne, "task 1", 1024, NULL, 2, NULL, app_cpu);
    xTaskCreatePinnedToCore(ledTwo, "task 2", 1024, NULL, 1, NULL, app_cpu);
    xTaskCreatePinnedToCore(ledTree, "task 3", 1024, NULL, 1, NULL, app_cpu);
    vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}