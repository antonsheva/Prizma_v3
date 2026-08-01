#include "../include/AN_taskRs485Poll.h"

void AN_taskRs485Poll::run(void *param){
  _MSG_PACK msg;
  AN_rs485 rs485;
  AN_shiftDataArr sft;
  for(;;){
    xQueueReceive(QueueRs485Pool, &msg, portMAX_DELAY);
    
    for(int i=0; i<msg.subscribersQty; i++){
      rs485.prepMsg(&msg, i);
      if(msg.addrEsp32 != G_lJmrStt.esp32Addr){       
        xQueueSend(QueueRs485Send, &msg, portMAX_DELAY);	
      } 
      vTaskDelay(200/portTICK_PERIOD_MS);
    }
    // xQueueSend(QueueBt, &msg, portMAX_DELAY);
    // ASetOccurredEvent(Event_finishLoadConfig);
    rs485.sendMsgToBt(&msg);
    AN_print("JMMR_LIST_LEN -> "+std::to_string(G_jmmrsList.size())); 
  }
}

AN_taskRs485Poll::AN_taskRs485Poll(/* args */)
{
  
}

AN_taskRs485Poll::~AN_taskRs485Poll()
{
}














