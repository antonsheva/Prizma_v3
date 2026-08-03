#include "AN_taskMonitor.h"
#include "main.h"

AN_taskMonitor::AN_taskMonitor(/* args */)
{
}

AN_taskMonitor::~AN_taskMonitor()
{
}

void AN_taskMonitor::checkAnalog(){
    static bool wschEnAnalogCheck = 0;
    if(!wschEnAnalogCheck){
      if(G_ledsStste[1]==2)wschEnAnalogCheck = 1;
    }else{
      if(!G_analogVolt){
        G_ledsStste[0]=2;
        G_ledsStste[1]=0;
      }else{
        G_ledsStste[0]=3;
        G_ledsStste[1]=0;        
      }
    }
}

void AN_taskMonitor::run(void *param){
  _MSG_PACK      msg;
  _MSG_INTERNAL  msg1;
  // RmCtrl *rmCtrl = RmCtrl::getI();
  int aut = 0;
  int cnt = 0;
  BYTE pwrOutVal = 0;
  BYTE prevPwrOutVal = 0;

  BYTE ledsCode[4] = {0};
 
  vTaskDelay(100/portTICK_PERIOD_MS);
  for(;;){

    switch(aut){
        case 0:   G_ledsStste[0]=1;
                  G_ledsStste[1]=0;
                  xQueueSend(QueueLeds, ledsCode, 10); break;

        case 20 :  msg.cmd = CMD_APLAY_PWR;
                  xQueueSend(QueueCmd, &msg, portMAX_DELAY);  break;
        case 30:  msg.cmd = CMD_RM_GET_STATE;
                  xQueueSend(QueueCmd, &msg, portMAX_DELAY);  break;
    }
    if(aut < 40) aut++;

    if(G_pwrMode == 0)checkAnalog();
    if(G_pwrMode == 1){
      G_ledsStste[0] = 4;
      G_ledsStste[1] = 0;
    }
    if(G_pwrMode == 2){
      G_ledsStste[0] = 5;
      G_ledsStste[1] = 0;
    }
    

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
        AN_print("G_waitBtConnect---- "); 
        msg1.code = EVENT_TIMEOUT_BT_CONNECT;
        xQueueSend(QueuePwrAut, &msg1, portMAX_DELAY);
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

    
    cnt++;
    // if(!(cnt%20)){
    //   std::string d = "cnt -> "+std::to_string(cnt);
    //   sPack.data = static_cast<char *>(malloc(sizeof(d)));  
    //   memccpy(sPack.data, d.c_str(), 0, 128);
    //   xQueueSend(QueueBt, &sPack, portMAX_DELAY);
    // }
    vTaskDelay(10/portTICK_PERIOD_MS);
  }  
}