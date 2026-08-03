#include "../include/AN_taskRs485Poll.h"

void AN_taskRs485Poll::run(void *param){
  _MSG_PACK msg;
  AN_rs485 rs485;
  AN_shiftDataArr sft;
  int cmdType;
  for(;;){
    xQueueReceive(QueueRs485Pool, &msg, portMAX_DELAY);
    cmdType = msg.cmdType;
 
    // sft.printJmmrList();
    for(int i=0; i<msg.subscribersQty; i++){
      rs485.prepMsg(&msg, i);
      // Serial.println("adEsp -> "+String(msg.addrEsp32)); 
      if(msg.addrEsp32 != G_lJmrStt.esp32Addr){  
        // Serial.println("send msg to -> "+String(msg.addrEsp32)); 
        xQueueSend(QueueRs485Send, &msg, portMAX_DELAY);	
      } 
      vTaskDelay(200/portTICK_PERIOD_MS);
    }
 
    rs485.sendMsgToBt(&msg);       
    if(cmdType == CMD_SET_JMMR_LIST){
      msg.cmd           = CMD_RM_SET_STATE;
      msg.direction     = MSG_DIR_REQUEST;
      msg.updtLocalJmmr = 0;
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














