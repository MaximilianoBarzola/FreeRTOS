#include <Arduino.h>
#define BUF 200
#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0; 
#else 
static const BaseType_t app_cpu = 1; 
#endif
boolean ready = false;
boolean head_create = false;
uint8_t counter = 0;
static char* direccion = NULL;

void readSerial(void* parameters){
    char x = ' ';
    for(;;){
        while(ready == false){
            if(Serial.available() > 0){
                if(head_create == false){
                    direccion = (char*)pvPortMalloc(BUF * sizeof(char));
                    configASSERT(direccion);  //Reinicio en el caso de que no haya suficiente memoria
                    memset(direccion, 0, BUF);
                    head_create = true;
                }
                x = Serial.read();
                if(x != '\n'){
                    //uint16_t aux = BUF;
                    if(counter < BUF - 1){
                        direccion[counter] = x;
                        counter++;
                    }
                    else{
                        Serial.println("Limit 200 characters");
                    }
                }
                if(x == '\n'){
                    ready =  true;
                    vTaskDelay(300 / portTICK_PERIOD_MS);
                }
            }
        }
        if(ready == true){
            Serial.println(direccion);
            vPortFree(direccion);
            Serial.print("Memoria de heap disponible: ");
            Serial.println(xPortGetFreeHeapSize());
            counter = 0;
            head_create = false;
            ready = false;
        }
    }
    vTaskDelete(NULL);
}
void printHeap(void* parameters){
    for(;;){
        //Serial.println("hola");
        if(ready == true){
            Serial.println("hola");
            Serial.println(direccion);
            vPortFree(direccion);
            Serial.print("Memoria de heap disponible: ");
            Serial.println(xPortGetFreeHeapSize());
            counter = 0;
            head_create = false;
            ready = false;
        }
    }
    vTaskDelete(NULL);
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