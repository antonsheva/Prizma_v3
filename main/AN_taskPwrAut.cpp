#include "../include/AN_taskPwrAut.h"
#include "AN_taskPwrAut.h"
  
static bool _btEnSwch;

static bool _ignr = 0;

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
  _btEnSwch = 0;
  Serial.println("EVENT_APPLY_CHANGES");
  _ignr = 1;
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
  if(_btEnSwch)return;
  _btEnSwch = 1;
  Serial.println("EVENT_BTTN_PRESS"); 
  JMMR_1_OFF
  JMMR_2_OFF
  msg.cmd = CMD_BT_START;
  xQueueSend(QueueCmd, &msg, portMAX_DELAY);  
  G_waitBtConnect = 3000; 
}

void AN_taskPwrAut::eventResumeWork(){
  if(G_btConnect)G_pwrMode = PWR_MODE_BT_CONNECT;
  else           G_pwrMode = PWR_MODE_MAIN; 
  _ignr = 0;   
}

void AN_taskPwrAut::run(void *param){
  _SERIAL_PACK sPack;
 
  _btEnSwch = 0;
    for(;;){
      xQueueReceive(QueuePwrAut, &sPack, portMAX_DELAY); 
      Serial.println("EVENT_CODE -> "+String(sPack.cmd));   
      switch (sPack.cmd){
        case EVENT_BTTN_PRESS         : if(!_ignr) eventBtOn();        break;          
        case EVENT_BT_CONNECT         : if(!_ignr) eventConnect();     break;          
        case EVENT_BT_DISCONNECT      : if(!_ignr) eventDisconnect();  break;        
        case EVENT_TIMEOUT_BT_CONNECT : if(!_ignr) eventPwrOff();      break;        
        case EVENT_APPLY_CHANGES      : if(!_ignr) eventAplayChanges(sPack.code); break;
        case EVENT_RESTART_ESP        : esp_restart();                break;
        case EVENT_RESUME_WORK        : eventResumeWork();            break;            
      }
  }
}



