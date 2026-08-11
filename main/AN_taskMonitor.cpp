#include "AN_taskMonitor.h"
#include "main.h"

AN_taskMonitor::AN_taskMonitor(/* args */)
{
}

AN_taskMonitor::~AN_taskMonitor()
{
}

void AN_taskMonitor::checkAnalog(){
    static BYTE cnt = 0;
    if(!(cnt%10)){
      if(!G_voltToLeds){
        G_ledsStste[0]=2;
        G_ledsStste[1]=0;
      }else{
        G_ledsStste[0]=3;
        G_ledsStste[1]=0;        
      }
    }
    cnt++;
}

void AN_taskMonitor::setLedsState(){
    if(G_pwrMode == PWR_MODE_START_ESP){
      if(G_ledsStste[0] != 1){
        G_ledsStste[0] = 1;
        G_ledsStste[1] = 0;
      }
      if(G_ledsStste[1] == 2){
        G_pwrMode = PWR_MODE_MAIN;
        G_ledsStste[0] = 0;
        G_ledsStste[1] = 0;
      }
    }
    if(G_pwrMode == PWR_MODE_MAIN){
      checkAnalog();
    }
    if(G_pwrMode == PWR_MODE_BT_WAIT_CONNECT){
      G_ledsStste[0] = 4;
      G_ledsStste[1] = 0;
    }
    if(G_pwrMode == PWR_MODE_BT_CONNECT){
      
      G_ledsStste[0] = 5;
      G_ledsStste[1] = 0;
    }
    if(G_pwrMode == PWR_MODE_PWR_OFF){
      
      G_ledsStste[0] = 6;
      G_ledsStste[1] = 0;
    }    
    if((G_pwrMode == PWR_MODE_APPLY_CHANGE)||(G_pwrMode == PWR_MODE_RESTART)){
      if( G_ledsStste[0]!=7){
        G_ledsStste[0] = 7;
        G_ledsStste[1] = 0;        
      }
      if(G_ledsStste[1] == 8){
        if(G_pwrMode == PWR_MODE_RESTART){
          Serial.println("Restart device");
          vTaskDelay(100);
          esp_restart();
        }else{
          Serial.println("Save state ");
          if(G_btConnect)G_pwrMode = PWR_MODE_BT_CONNECT;
          else           G_pwrMode = PWR_MODE_MAIN;          
        }
        G_ledsStste[1] = 0; 
      }

    }      
 
}

void AN_taskMonitor::run(void *param){
  _MSG_PACK msg;
  // RmCtrl *rmCtrl = RmCtrl::getI();
  int aut = 0;
  int cnt = 0;
  BYTE pwrOutVal = 0;
  BYTE prevPwrOutVal = 0;

  BYTE ledsCode[4] = {0};
 
  vTaskDelay(100/portTICK_PERIOD_MS);
  for(;;){

    switch(aut){
        case 20 :  msg.cmd = CMD_APLAY_PWR;
                  xQueueSend(QueueCmd, &msg, portMAX_DELAY);  break;
        case 30:  msg.cmd = CMD_RM_GET_STATE;
                  xQueueSend(QueueCmd, &msg, portMAX_DELAY);  break;
    }
    if(aut < 40) aut++;

    setLedsState();

//////////////////////////////////////////////////


    if(G_pauseRmDataCnt)G_pauseRmDataCnt--;
    if(G_pauseRmDataCnt == 2){
        vTaskResume(Handle_taskRmReceive);

    }
    if(G_rebModAut_tm < 100)G_rebModAut_tm++;
    if(G_rebModAut_tm == 95){
        vTaskResume(Handle_taskRmReceive);
    }

    if(G_wait485PackCnt < 10)G_wait485PackCnt++;
    if(G_waitBtPackCnt  < 10)G_waitBtPackCnt++;

    if(G_waitBtConnect)G_waitBtConnect--;
    if(G_waitBtConnect == 2){
        _SERIAL_PACK sPack;
        sPack.cmd = EVENT_TIMEOUT_BT_CONNECT;
        xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);
    }

    
    if(!(cnt%11)){
      pwrOutVal = ~(gpio_get_level(PIN_CN1) | (gpio_get_level(PIN_CN2)<<1));
      pwrOutVal &= 0x03;
      if(pwrOutVal != prevPwrOutVal){
        prevPwrOutVal = pwrOutVal;
        ledsCode[0]=6;
        ledsCode[1]=pwrOutVal;
        xQueueSend(QueueLeds, ledsCode, 10);        
      }

    }    
 
    vTaskDelay(10/portTICK_PERIOD_MS);
  }  
}