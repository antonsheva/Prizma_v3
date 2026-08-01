#include "../include/AN_taskRmReceive.h"
 

AN_taskRmReceive::AN_taskRmReceive(/* args */){}
AN_taskRmReceive::~AN_taskRmReceive(){}



void AN_taskRmReceive::callback(){
  _SERIAL_PACK sPack;
  int len = Serial1.available();
  sPack.data = static_cast<char *>(malloc(len));
  if(sPack.data == NULL) {
        ESP_LOGE("UART", "Malloc failed");
    return;
  }
  Serial1.read(sPack.data, len);
  sPack.len = len;
  xQueueSend(QueueRmEvent, &sPack, portMAX_DELAY);
}

void AN_taskRmReceive::run(void *param){
  _SERIAL_PACK sPack;
  for(;;){
    if (xQueueReceive(QueueRmEvent, &sPack, (TickType_t)portMAX_DELAY)) {
        // Serial.println(" - - data from RM - - -");
        Serial.write(sPack.data, sPack.len);
        free(sPack.data);
      
    }
  }
}





