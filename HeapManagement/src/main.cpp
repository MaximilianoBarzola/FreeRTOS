#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
#define BUF 255
static char *msg = NULL;
static volatile bool flag = 0;


void readSerial(void *parameters) {
    char c;
    char buf[BUF];
    byte idx = 0;
    memset(buf, 0, BUF);
    for(;;){
        if(flag == 0){
            if(Serial.available() > 0){
                c = Serial.read();
                if(c != '\n' && idx < BUF){
                    buf[idx] = c;
                    idx++;
                }
                else if('\n' == c){
                    if (msg == NULL){
                        msg = (char *)pvPortMalloc(sizeof(buf)* sizeof(char));
                        configASSERT(msg);
                    }
                    memcpy( msg, buf, sizeof(buf));
                    memset(buf, 0, idx);
                    idx = 0;
                    flag = 1;
                }
            }
        }
    }
    vTaskDelete(NULL);
}


void printMessage(void *parameters) {
    for(;;){
        if(flag){
            Serial.println(msg);
            vPortFree(msg);
            msg = NULL;
            flag = 0;
        }   
    }
    vTaskDelete(NULL);
}

void setup() {  

    Serial.begin(115200);   

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println();
    Serial.println("---FreeRTOS Heap Demo---");
    Serial.println("Enter a string");

    xTaskCreatePinnedToCore(readSerial,"Read Serial",1024,NULL,1,NULL,app_cpu);
    xTaskCreatePinnedToCore(printMessage,"Print Message",1024,NULL,1,NULL,app_cpu);

    vTaskDelete(NULL);
}

void loop() {

}