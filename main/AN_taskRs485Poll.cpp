#include "../include/AN_taskRs485Poll.h"

void AN_taskRs485Poll::run(void *param){
  _MSG_PACK msg;
  AN_commRs485Bt rs485;
  AN_shiftDataArr sft;
  int cmdType;
  for(;;){
    xQueueReceive(QueueRs485Pool, &msg, portMAX_DELAY);
    cmdType = msg.cmdType;
    for(int i=0; i<msg.subscribersQty; i++){
      rs485.prepMsg(&msg, i);
      if(msg.addrEsp32 != G_lJmrStt.esp32Addr){   
        xQueueSend(QueueRs485Send, &msg, portMAX_DELAY);	
      } 
      vTaskDelay(200/portTICK_PERIOD_MS);
    }
    msg.cmdType = cmdType;
    rs485.sendMsgToBt(&msg);       
    if(cmdType == CMD_SET_JMMR_LIST){
      msg.cmd           = CMD_SET_JMMR_DATA;
      msg.direction     = MSG_DIR_REQUEST;
      sft.loadJmmrStateToMsg(&msg, &G_lJmrStt);
      xQueueSend(QueueCmd, &msg, portMAX_DELAY);      
    }
  }
}

AN_taskRs485Poll::AN_taskRs485Poll(/* args */)
{
  
}

AN_taskRs485Poll::~AN_taskRs485Poll()
{
}














