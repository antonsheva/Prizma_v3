#include "../include/AN_taskPwrAut.h"
#include "AN_taskPwrAut.h"
 
static BYTE  ledsCode[4];  
static bool  btEnSwch;

AN_taskPwrAut::AN_taskPwrAut(/* args */){}
AN_taskPwrAut::~AN_taskPwrAut(){}

void AN_taskPwrAut::eventPwrOff(){
  _MSG_PACK msg;
  Serial.println("EVENT_TIMEOUT_BT_CONNECT");
  msg.cmd = CMD_BT_STOP;
  xQueueSend(QueueCmd, &msg, portMAX_DELAY);
  G_pwrMode = PWR_MODE_PWR_OFF;       
  for(;;){gpio_set_level(PIN_PWR_HOLD_DRV, 0);}
}

void AN_taskPwrAut::eventAplayChanges(int code){
  G_pwrMode = PWR_MODE_APPLY_CHANGE;
  if(code == CMD_RESTART_ESP)G_pwrMode = PWR_MODE_RESTART;
  btEnSwch = 0;
  Serial.println("EVENT_APPLY_CHANGES");
}
void AN_taskPwrAut::eventDisconnect(){
  Serial.println("EVENT_BT_DISCONNECT");
  G_pwrMode = PWR_MODE_BT_WAIT_CONNECT; 
  G_waitBtConnect = 3000;
}

void AN_taskPwrAut::eventConnect(){
  Serial.println("EVENT_BT_CONNECT"); 
  G_pwrMode = PWR_MODE_BT_CONNECT; 
  G_waitBtConnect = 0;
}

void AN_taskPwrAut::eventBtOn(){
  _MSG_PACK msg;
  G_pwrMode = PWR_MODE_BT_WAIT_CONNECT; 
  if(btEnSwch)return;
  btEnSwch = 1;
  Serial.println("EVENT_CODE_BTTN_ON"); 
  JMMR_1_OFF
  JMMR_2_OFF
  msg.cmd = CMD_BT_START;
  xQueueSend(QueueCmd, &msg, portMAX_DELAY);  

  G_waitBtConnect = 3000; 
}
 

void AN_taskPwrAut::run(void *param){
  _SERIAL_PACK sPack;
  for(int i=0; i<4; i++)ledsCode[i] = 0;
  btEnSwch = 0;
    for(;;){
        xQueueReceive(QueuePwrAut, &sPack, portMAX_DELAY); 
        if(G_pwrMode == PWR_MODE_RESTART) return;
        if(G_pwrMode == PWR_MODE_PWR_OFF) return;
        
        Serial.println("EVENT_CODE -> "+String(sPack.cmd));   
        switch (sPack.cmd){
            case EVENT_CODE_BTTN_ON       : eventBtOn();                  break;          
            case EVENT_BT_CONNECT         : eventConnect();               break;          
            case EVENT_BT_DISCONNECT      : eventDisconnect();            break;        
            case EVENT_TIMEOUT_BT_CONNECT : eventPwrOff();                break;        
            case EVENT_APPLY_CHANGES      : eventAplayChanges(sPack.code);break;
        }
  }
}



