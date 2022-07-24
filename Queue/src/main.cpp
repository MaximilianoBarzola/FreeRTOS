#include <Arduino.h>
#define QUEUE_LEN 5
#define LED 2
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
struct msg{
  String msg = String();
  uint16_t num;
};

static QueueHandle_t delay_value;
static QueueHandle_t msg_queue;

void serialRead(void *pvParameters){
  msg receive;
  char x;
  String command;
  uint16_t num;
  for(;;){
    if(Serial.available() > 0){
      x = Serial.read();
      if(x != '\n'){
        command = command + x;
        Serial.print(x);
        if(x == '\b'){
          Serial.print(' ');
          command = command + ' ';
          Serial.print('\b');
          command = command + '\b';

        }
      }
      else{
        String sub = String();
        uint32_t delay;
        Serial.print('\n');
        //Serial.println(command);
        if(command.indexOf("delay") > -1){
          sub = command.substring(sizeof("delay"), sizeof(command));
          delay = sub.toInt();
          if(delay > 0){
            xQueueSend(delay_value, (void*)&delay, 10);
          }
        }
        command = "";
      }
    }
    if(xQueueReceive(msg_queue, (void*)&receive, 0) == pdTRUE){
      Serial.println("");
      Serial.println(receive.msg);
      Serial.println(receive.num);
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void ledControl(void *pvParameters){
  msg Message;
  Message.msg = "Blink";
  Message.num = 0;
  uint32_t delay  = 1000;
  uint64_t time = millis();

  for(;;){
    if(millis() - time > delay){
      digitalWrite(LED, !digitalRead(LED));
      time = millis();
      if(Message.num < 100){
        Message.num++;
      }
      else{
        xQueueSend(msg_queue, (void*)&Message, 10);
        Message.num = 0;
      }
    }
    xQueueReceive(delay_value, (void*)&delay, 0);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void setup(){
  Serial.begin(115200);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  //create queue
  pinMode(LED, OUTPUT);
  delay_value = xQueueCreate(QUEUE_LEN, sizeof(int));
  msg_queue = xQueueCreate(QUEUE_LEN, sizeof(msg));
  xTaskCreatePinnedToCore(serialRead, "Terminal", 1024, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(ledControl, "Blink", 1024, NULL, 1, NULL, app_cpu);
  vTaskDelete(NULL);
}

void loop(){

}