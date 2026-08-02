#include "../include/AN_taskRs485Poll.h"

void AN_taskRs485Poll::run(void *param){
  _MSG_PACK msg;
  AN_rs485 rs485;
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
 
    rs485.sendMsgToBt(&msg);       
    if(cmdType == CMD_SET_JMMR_LIST){
      Serial.println("tp - 1");
      msg.cmd       = CMD_RM_SET_STATE;
      msg.direction = MSG_DIR_REQUEST;
      xQueueSend(QueueCmd, &msg, portMAX_DELAY);      
    }

    AN_print("JMMR_LIST_LEN -> "+std::to_string(G_jmmrsList.size())); 
    sft.printJmmrList();
  }
}

AN_taskRs485Poll::AN_taskRs485Poll(/* args */)
{
  
}

AN_taskRs485Poll::~AN_taskRs485Poll()
{
}














