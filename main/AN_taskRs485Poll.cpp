#include "../include/AN_taskRs485Poll.h"
#include <vector>




int AN_taskRs485Poll::checkStopPoll(_MSG_PACK *msg){ 
  if(msg->cmdType == CMD_GET_JMMR_LIST){
    Serial.println("devQty -> "+String(msg->devQty));
    if(G_foundSubscribers >=  msg->devQty) return POLL_STATE_STOP;
    else                                        return POLL_STATE_ERROR;
  }
  return POLL_STATE_CONTINUE;
}

bool AN_taskRs485Poll::checkExistAddr(BYTE addr){
  for(int i=0; i<G_foundAddr.size(); i++){
    // Serial.println("addr -> "+String(addr));
    if(G_foundAddr[i] == addr){
      // Serial.println("Found addr -> "+String(G_foundAddr[i]));
      return true;
    }
  }
  return false;
}


void AN_taskRs485Poll::run(void *param){
  _MSG_PACK msg;
  AN_commRs485Bt rs485;
  AN_shiftDataArr sft;
  int cmdType;
  int needBtOff = 0;
  int period;
  int error = 1;
 
  for(;;){
    xQueueReceive(QueueRs485Pool, &msg, portMAX_DELAY);
    cmdType   = msg.cmdType;
    needBtOff = msg.needBtOff;
    for(BYTE t = 0; t < MAX_TRY_QTY; t++){
      for(BYTE i=0; i<msg.subscribersQty; i++){
        if(t>0)if(checkExistAddr(i+1))continue;
        
        rs485.prepMsg(&msg, i);
        if(msg.addrEsp32 != G_lJmrStt.esp32Addr){   
          xQueueSend(QueueRs485Send, &msg, portMAX_DELAY);	
        } 
        period = (msg.addressee == BROADCAST_ADDR) ? 50 : 200;
        if(t>0)if(checkStopPoll(&msg)==POLL_STATE_STOP)break;
        vTaskDelay(period/portTICK_PERIOD_MS);
      }      

      if(checkStopPoll(&msg)!= POLL_STATE_ERROR){
        error = 0;
        break;
      }else{
        Serial.println("Try search device "+String(t));
      }
    }
    
    msg.cmdType = cmdType;
    msg.response = error == 0 ? RESP_OK : RESP_ERROR;
    rs485.sendMsgToBt(&msg);       
    if(cmdType == CMD_SET_JMMR_LIST){
      msg.cmd           = CMD_SET_JMMR_DATA;
      msg.direction     = MSG_DIR_REQUEST;
      msg.needBtOff     = needBtOff;
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














                           