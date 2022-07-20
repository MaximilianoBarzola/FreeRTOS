#include <Arduino.h>
#define QUEUE_LEN 5
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
struct msg{
  String msg;
  uint16_t num;
};

static QueueHandle_t delay_value;
static QueueHandle_t msg_queue;

void serialRead(void *pvParameters){
  msg receive;
  for(;;){
    if(xQueueReceive(msg_queue, (void*)&receive, 0) == pdTRUE){
      Serial.println(receive.msg);
      Serial.println(receive.num);
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void ledControl(void *pvParameters){
  msg Message; 
  Message.msg = "hello";
  Message.num = 0;
  for(;;){
    //Serial.println("hola2");
    xQueueSend(msg_queue, (void*)&Message, 10);
    Message.num++;
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void setup(){
  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  //create queue
  delay_value = xQueueCreate(QUEUE_LEN, sizeof(int));
  msg_queue = xQueueCreate(QUEUE_LEN, sizeof(msg));
  xTaskCreatePinnedToCore(serialRead, "Terminal", 1024, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(ledControl, "Blink", 1024, NULL, 1, NULL, app_cpu);
  vTaskDelete(NULL);
}

void loop(){

}