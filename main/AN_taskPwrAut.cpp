#include "../include/AN_taskPwrAut.h"
 
static BYTE  ledsCode[4];  
static bool  btEnSwch;

AN_taskPwrAut::AN_taskPwrAut(/* args */){}
AN_taskPwrAut::~AN_taskPwrAut(){}

void AN_taskPwrAut::eventPwrOff(){
  _MSG_PACK msg;
  AN_print("EVENT_PWR_OFF_BTTN");
  msg.cmd = CMD_BT_STOP;
  xQueueSend(QueueCmd, &msg, portMAX_DELAY);
  ledsCode[0]=0;
  xQueueSend(QueueLeds, &ledsCode, portMAX_DELAY);      
  for(;;){gpio_set_level(PIN_PWR_HOLD_DRV, 0);}
}

void AN_taskPwrAut::eventAplayChanges(){
  _MSG_PACK msg;  

  btEnSwch = 0;
  G_btConnect = 0;
  G_pwrMode = 0;
  ledsCode[0]=1;
  xQueueSend(QueueLeds, ledsCode, 100);
}
void AN_taskPwrAut::eventDisconnect(){
  AN_print("BT disconnect");
  G_waitBtConnect = 600;
  ledsCode[0]=2;
  xQueueSend(QueueLeds, &ledsCode, portMAX_DELAY);
}

void AN_taskPwrAut::eventConnect(){
  AN_print("BT connect");
  G_waitBtConnect = 0;
  ledsCode[0]=3;
  xQueueSend(QueueLeds, ledsCode, portMAX_DELAY); 
}

void AN_taskPwrAut::eventBtOn(){
  _MSG_PACK msg;
  G_pwrMode = 1; 
  if(btEnSwch)return;
  btEnSwch = 1;
  AN_print("BT on");
  JMMR_1_OFF
  JMMR_2_OFF
  msg.cmd = CMD_BT_START;
  xQueueSend(QueueCmd, &msg, portMAX_DELAY);            
  G_waitBtConnect = 3000; //x 10mSec; if don't connect to bluetooth during this time - turn off the power
  ledsCode[0]=2;
  xQueueSend(QueueLeds, ledsCode, portMAX_DELAY); 
}
 

void AN_taskPwrAut::run(void *param){
  _SERIAL_PACK sPack;
  for(int i=0; i<4; i++)ledsCode[i] = 0;
  btEnSwch = 0;
    for(;;){
        xQueueReceive(QueuePwrAut, &sPack, portMAX_DELAY); 
        G_led_ccl_5 = 0;
        Serial.println("EVENT_CODE -> "+String(sPack.cmd));   
        switch (sPack.cmd){
            case EVENT_CODE_BTTN_ON       : eventBtOn();
                  Serial.println("EVENT_CODE_BTTN_ON");              break;          
            case EVENT_BT_CONNECT         : eventConnect();
                  Serial.println("EVENT_BT_CONNECT");           break;          
            case EVENT_BT_DISCONNECT      : eventDisconnect();
                  Serial.println("EVENT_BT_DISCONNECT");        break;        
            case EVENT_TIMEOUT_BT_CONNECT : eventPwrOff();
                  Serial.println("EVENT_TIMEOUT_BT_CONNECT");           break;        
            case EVENT_APPLY_CHANGES      : eventAplayChanges();
                  Serial.println("EVENT_APPLY_CHANGES");      break;
        }
  }
}



