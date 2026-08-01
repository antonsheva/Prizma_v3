 
#include "../include/AN_taskUsb.h"



void AN_taskUsb::callback(){
  _SERIAL_PACK sPack;
  int len = Serial.available();
  sPack.data = static_cast<char *>(malloc(len));
  if(sPack.data == NULL) {
        ESP_LOGE("UART", "Malloc failed");
    return;
  }
  Serial.read(sPack.data, len);
  sPack.len = len;
  xQueueSend(QueueUsb, &sPack, portMAX_DELAY);
}
 

void AN_taskUsb::run(void *param){
  _SERIAL_PACK sPack;
  AN_serial serial;
  for (;;){
      xQueueReceive(QueueUsb, &sPack, portMAX_DELAY); 

      serial.processingSerialData(sPack);
      Serial.println(" - - data from USB - - -");
      Serial.write(sPack.data, sPack.len);
      // Serial1.write(sPack.data, sPack.len);
      // free(sPack.data); 
  }
}

AN_taskUsb::AN_taskUsb(/* args */)
{
  
}

AN_taskUsb::~AN_taskUsb()
{
}














