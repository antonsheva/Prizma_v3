#include "../include/AN_taskRmAut.h"
#include "AN_taskRmAut.h"


AN_taskRmAut::AN_taskRmAut(/* args */){}

AN_taskRmAut::~AN_taskRmAut(){}

void AN_taskRmAut::run(void *param){
   _RM_AUT rmAut;  
  BYTE opCode;
  // RmCtrl *rmCtrl = RmCtrl::getI();   
  for(;;){
    xQueueReceive(QueueRebModAut, &rmAut, portMAX_DELAY);
    // // rmCtrl->isBusy = true;
    // for(int i=0; i<rmAut.opCodeQty; i++){
      
    //     // switch(rmAut.opCodeList[i]){       
    //     //     case CMD_AT       : RmCtrl::getI()->At();       break;
    //     //     case CMD_GET_ATBT : RmCtrl::getI()->getAtbt();  break;
    //     //     case CMD_GET_ATC  : RmCtrl::getI()->getAtc();   break;
    //     //     case CMD_SET_ATC  : RmCtrl::getI()->setAtc();   break;
    //     //     case CMD_GET_ATI  : RmCtrl::getI()->getAti();   break;
    //     //     case CMD_ATZ      : RmCtrl::getI()->setAtz();   break;
    //     //     case CMD_SET_ATW  : RmCtrl::getI()->setAtw();   break;
    //     //     case CMD_SET_ATE0 : RmCtrl::getI()->setAte0();  break;
    //     //     case CMD_SET_ATE1 : RmCtrl::getI()->setAte1();  break;
    //     // }
    //     G_rebModAut_tm = 0; 
    //   Serial.println("------ stt 1 ---------");                    
    //     vTaskSuspend(NULL);

    //   Serial.println("------ stt 2 ---------");                    

    //     // for(int i=0; i<RM_BUFF_LEN; i++)rmCtrl->inData[i] = 0;
    //     int len = Serial1.available();
    //     // Serial1.read(rmCtrl->inData, len);
    //     Serial1.flush();

    //     // rmCtrl->getDevInfo(String(rmCtrl->inData)); 
    //     Serial.println ();
    //     Serial.println("------ REBMOD DATA ---------");
    //     Serial.println(String(rmCtrl->inData));
    //     Serial.println("----------------------------");
    //     Serial.println ();
    //     if(rmAut.swtchActDev)
    //         rmCtrl->selDev = (rmCtrl->selDev==0) ? 1 : 0;
    // }
    // rmCtrl->isBusy = false;
 
     
    // if(rmCtrl->cmdAfterAutFinish == CMD_RESTART_ESP)esp_restart();
  } 
}
