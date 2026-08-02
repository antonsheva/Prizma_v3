#include "AN_taskBtReceive.h"

AN_taskBtReceive::AN_taskBtReceive(/* args */){}
AN_taskBtReceive::~AN_taskBtReceive(){}

void AN_taskBtReceive::run(void *param){
  _SERIAL_PACK sPack;
  AN_serial serial;
  AN_bt bt;
  for(;;){
    if(xQueueReceive(QueueBtReceive, &sPack, portMAX_DELAY)){
        if(G_btConnect){
          serial.dataSrc = SERIAL_SRC_BT; 
          G_waitBtPackCnt = 0;              
          serial.processingSerialData(sPack);
           
        }
    }
  }
}