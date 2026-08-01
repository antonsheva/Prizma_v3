#include "../include/AN_taskBt.h"
 
 
  
 



 

  

 

// void AN_bt::saveConnectionData(esp_spp_cb_param_t *param){
//     memccpy(G_lJmrStt.bt.rem_bda, param->open.rem_bda, 0, ESP_BD_ADDR_LEN);
//     G_lJmrStt.bt.sppClient = param->open.handle;
//     G_lJmrStt.bt.status = SPP_CONNECTED;
//     G_btConnect = true;
// }

// void AN_bt::clearConnectionData(){
//     memset(G_lJmrStt.bt.rem_bda, 0, ESP_BD_ADDR_LEN);
//     G_lJmrStt.bt.sppClient = 0;
//     G_lJmrStt.bt.status = SPP_DISCONNECTED;
// } 

BluetoothSerial SerialBT; 
AN_serial serial;
 
AN_taskBt::AN_taskBt(/* args */){
  SerialBT.register_callback(callback);
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

  sPack.len = SerialBT.available();
  sPack.data = static_cast<char *>(malloc(sPack.len)); 
 
  for(int i=0; i<sPack.len; i++)sPack.data[i] = SerialBT.read();
  serial.dataSrc = SERIAL_SRC_BT; 
  G_waitBtPackCnt = 0;              
  serial.processingSerialData(sPack);
  // free(sPack.data);
}

void AN_taskBt::callback(esp_spp_cb_event_t event, esp_spp_cb_param_t * param){
    _SERIAL_PACK sPack;
   
    Serial.println("ESP_SPP_EVT ->  "+String(event));
    if (event == ESP_SPP_START_EVT) {
        Serial.println("ESP_SPP_OPEN_EVT");
    }
    
    if (event == ESP_SPP_SRV_OPEN_EVT) {
        sPack.cmd = EVENT_BT_CONNECT;
        Serial.println("ESP_SPP_SRV_OPEN_EVT Connected");
        xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY); 
        G_btConnect = true;
    }    
    
    if (event == ESP_SPP_CLOSE_EVT) {
        sPack.cmd = EVENT_BT_DISCONNECT;
        Serial.println("ESP_SPP_CLOSE_EVT disconnected");
        xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY); 
        G_btConnect = false; 
    }

    if(event == ESP_SPP_DATA_IND_EVT){ 
        Serial.println("ESP_SPP_DATA_IND_EVT  new data");
        receive(); 
    }
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