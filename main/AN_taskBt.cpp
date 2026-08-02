#include "AN_taskBt.h"
 
 


AN_serial serial;
 
AN_taskBt::AN_taskBt(/* args */){

}
AN_taskBt::~AN_taskBt(){
  stop();
}

void AN_taskBt::start(){
  String devName;
  if(G_lJmrStt.devType == DEV_TYPE_A)devName = "Prizma_JMR_A_"+String(G_lJmrStt.esp32Addr);
  else                               devName = "Prizma_JMR_A_"+String(G_lJmrStt.esp32Addr);
  SerialBT.begin(devName); 

}

void AN_taskBt::stop(){

}
void AN_taskBt::send(_SERIAL_PACK sPack){
  SerialBT.write((uint8_t*)sPack.data, sPack.len);
  free(sPack.data);
}

void AN_taskBt::receive(){
  _SERIAL_PACK sPack;
  sPack.len = 0;
  sPack.data = static_cast<char *>(malloc(128)); 
  memset(sPack.data, 0, 128);
  memset(sPack.data, 0, 128); 
  while(SerialBT.available()){
    sPack.data[sPack.len++] = SerialBT.read();
    if(sPack.len >120)break;
  }

  serial.dataSrc = SERIAL_SRC_BT; 
  G_waitBtPackCnt = 0;   
  
  serial.processingSerialData(sPack);
  // free(sPack.data);
}



void AN_taskBt::run(void *data){
    _SERIAL_PACK sPack;
    for(;;){
      if(xQueueReceive(QueueBt, &sPack, portMAX_DELAY)){
        switch(sPack.cmd){
          case CMD_BT_START   : start();        break;
          case CMD_BT_STOP    : stop();         break;
          case CMD_BT_SEND    : send(sPack);    break;
          case CMD_BT_RECEIVE : receive(); break;
 
        }
 
      }
    }
}