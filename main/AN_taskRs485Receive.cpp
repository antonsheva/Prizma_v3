#include "../include/AN_taskRs485Receive.h"
 
AN_taskRs485Receive::AN_taskRs485Receive(/* args */)
{
}

AN_taskRs485Receive::~AN_taskRs485Receive()
{
}

void AN_taskRs485Receive::callback(){
  _SERIAL_PACK sPack;
  sPack.len = Serial2.available();
  if(sPack.len > 120)sPack.len = 120;
  sPack.data = static_cast<char *>(malloc(sPack.len));
  if(sPack.data == NULL) {
    ESP_LOGE("UART", "AN_taskRs485Receive Malloc failed ");
    return;
  }
  memset(sPack.data, 0, sPack.len);
  Serial2.read(sPack.data, sPack.len);

  xQueueSend(QueueRs485, &sPack, portMAX_DELAY);
}

void AN_taskRs485Receive::run(void *param){
  AN_serial serial;
  _SERIAL_PACK sPack;
  for(;;){
    if (xQueueReceive(QueueRs485, &sPack, portMAX_DELAY)){
        serial.dataSrc = SERIAL_SRC_485; 
        G_wait485PackCnt = 0;   
            
        serial.processingSerialData(sPack); 
    }
  }
}


