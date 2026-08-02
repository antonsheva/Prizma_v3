#include "AN_rs485.h"
#include "FastCRC.h" 

#define LOAD_RM_ONLY 1
void AN_rs485::processMsg(_MSG_PACK *msg)
{

  switch(msg->cmd){
      // case CMD_GET_JMMR_LIST: msg->cmd = CMD_SEARCH_DEVICES; break;
      case CMD_RM_SET_STATE : loadMsgToJmrStt(msg, &G_lJmrStt);         break;
  }
  xQueueSend(QueueCmd, &msg, 100);
}

void AN_rs485::prepMsg(_MSG_PACK *msg, BYTE iterNum)
{
  AN_shiftDataArr sft;
  switch(msg->cmdType){
    case CMD_GET_JMMR_LIST: msg->addrEsp32 = iterNum+1; 
                            msg->cmd       = CMD_SEARCH_DEVICES;
                            msg->direction = MSG_DIR_REQUEST;
                            msg->response  = RESP_GET_JMMR_LIST;                                     
                            break;
    case CMD_SET_JMMR_LIST: 
                            if(iterNum == G_lJmrStt.esp32Addr){
                              sft.loadMsgToJmrStt(msg, &G_lJmrStt, LOAD_RM_ONLY);  
                            }else{
                              msg->cmd        = CMD_RM_SET_STATE;                  
                              msg->direction  = MSG_DIR_REQUEST;
                              msg->addrEsp32  = G_jmmrsList[iterNum].esp32Addr;
                              loadJmmrStateToMsg(msg, &G_jmmrsList[iterNum]); 
                            }

                            break;                                
    case CMD_GET_JMMR_DATA: msg->cmd       = CMD_SEARCH_DEVICES;
                            msg->direction = MSG_DIR_REQUEST;   
                            msg->response  = RESP_GET_JMMR_DATA;                             
                            break;
    case CMD_SET_JMMR_DATA: msg->cmd        = CMD_RM_SET_STATE;                  
                            msg->direction  = MSG_DIR_REQUEST;
                            msg->response   = RESP_SET_JMMR_DATA;   
                            break; 
  }
}

void AN_rs485::sendBtData(char *data){
    // int len = sprintf(G_txtJsonBuff, "start___%s_stop", data);
     
	// Serial.println(" --- send to BT ---");
    // Serial.write(G_txtJsonBuff, len);
    // SerialBT.write((const uint8_t*)G_txtJsonBuff,len);
 
}

void AN_rs485::sendJammListToBt(){
    // int result = AN_json::getI()->packJmmrList();
    // sendBtData(G_txtSerialBuff);
}

void AN_rs485::sendBtResponse(BYTE cmd, uint32_t resp){
    // int result = AN_json::getI()->packResponse(cmd, resp);
    // sendBtData(G_txtSerialBuff);
}

void AN_rs485::sendBtJmmrData(_MSG_PACK *msg){
    // int result = AN_json::getI()->packJmmrData(msg);
    // sendBtData(G_txtSerialBuff);
}

int AN_rs485::transmitdataToBt(_SERIAL_PACK *sPack){
    if(G_btConnect){
        // xQueueSend(QueueBt, sPack, portMAX_DELAY);      
        return 0;
    }
    free(sPack->data);
    AN_print("Connection is missing");
    return -1;
}

void AN_rs485::selectUart(int uart){
    
}

void AN_rs485::sendMsgToBt(_MSG_PACK *msg){
    AN_serialConv serialConv;
	AN_print("--- q1 ---");
    _SERIAL_PACK sPack;
    int len = 0;
	switch (msg->cmdType){
		case CMD_GET_JMMR_LIST: 
            sPack.data =  static_cast<char *>(malloc(MAX_SERIAL_DATA_LEN));
            if (sPack.data == NULL) {
                ESP_LOGE("UART", "serializeJmmrList: malloc failed");
                return;
            }

            sPack.len = serialConv.serializeJmmrList(G_jmmrsList, sPack.data);

            if(G_btConnect){
                xQueueSend(QueueBtSend, &sPack, portMAX_DELAY);      
                return;
            }
            free(sPack.data);
            // sPack.cmd = CMD_BT_SEND;  
            // xQueueSend(QueueBt, &sPack, portMAX_DELAY);
            
        break; 
		
    case CMD_SET_JMMR_LIST: sendBtResponse(msg->cmdType, 1); break;
    case CMD_GET_JMMR_DATA: sendBtJmmrData(msg);        break;
	}
	msg->cmdType = 0; 
}
 
void AN_rs485::init()
{
     
}









    
 

 