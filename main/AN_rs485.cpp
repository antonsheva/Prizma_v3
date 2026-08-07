#include "AN_rs485.h"
#include "FastCRC.h" 

#define LOAD_RM_ONLY 1


void AN_commRs485Bt::prepMsg(_MSG_PACK *msg, BYTE iterNum)
{
  AN_shiftDataArr sft;
  switch(msg->cmdType){
    case CMD_GET_JMMR_LIST: msg->addressee = iterNum+1; 
                            msg->cmd       = CMD_GET_JMMR_DATA;
                            msg->direction = MSG_DIR_REQUEST;
                            msg->response  = RESP_GET_JMMR_LIST;                                     
                            break;

    case CMD_SET_JMMR_LIST: if(G_lJmrStt.esp32Addr == G_jmmrsList[iterNum].esp32Addr){ 
                              copyJmmr(&G_lJmrStt, &G_jmmrsList[iterNum], 1);
                              msg->addrEsp32 = G_lJmrStt.esp32Addr;
                            }else{
                              msg->cmd        = CMD_SET_JMMR_DATA;                  
                              msg->direction  = MSG_DIR_REQUEST;
                              msg->addressee  = G_jmmrsList[iterNum].esp32Addr;
                              loadJmmrStateToMsg(msg, &G_jmmrsList[iterNum]);                               
                            }

                            break;  

    case CMD_GET_JMMR_DATA: msg->cmd       = CMD_GET_JMMR_DATA;
                            msg->direction = MSG_DIR_REQUEST;   
                            msg->response  = RESP_GET_JMMR_DATA;                             
                            break;
    case CMD_SET_JMMR_DATA: msg->cmd        = CMD_SET_JMMR_DATA;                  
                            msg->direction  = MSG_DIR_REQUEST;
                            msg->response   = RESP_SET_JMMR_DATA;   
                            break; 
  }
}

void AN_commRs485Bt::sendBtJammList(){
  _SERIAL_PACK sPack;
  AN_serialConv serialConv;
  sPack.data =  static_cast<char *>(malloc(MAX_SERIAL_DATA_LEN));
  if (sPack.data == NULL) {
      ESP_LOGE("UART", "serializeJmmrList: malloc failed");
      return;
  }
  sPack.len = serialConv.serializeJmmrList(G_jmmrsList, sPack.data);
  Serial.println(" ----QueueBtSend ----");
  Serial.println(sPack.data);
  if(G_btConnect){
      xQueueSend(QueueBtSend, &sPack, portMAX_DELAY);      
      return;
  }
  Serial.println("BT connection is missing");
  free(sPack.data); 
}

void AN_commRs485Bt::sendBtResponse(BYTE cmd, uint32_t resp){
    // int result = AN_json::getI()->packResponse(cmd, resp);
    // sendBtData(G_txtSerialBuff);
}

void AN_commRs485Bt::sendBtJmmrData(_MSG_PACK *msg){
  _JMMR_STATE jmmr;
  AN_shiftDataArr sft;
  sft.loadMsgToJmrStt(msg, &jmmr);
  G_jmmrsList.clear();
  G_jmmrsList.push_back(jmmr);
  sendBtJammList();
}

void AN_commRs485Bt::sendBtJmmrData(_JMMR_STATE *jmmr1){
  _JMMR_STATE jmmr;
  AN_shiftDataArr sft;
  sft.copyJmmr(&jmmr, jmmr1);
  G_jmmrsList.clear();
  G_jmmrsList.push_back(jmmr);
  sendBtJammList();
}

void AN_commRs485Bt::sendMsgToBt(_MSG_PACK *msg){
	switch (msg->cmdType){
		case CMD_GET_JMMR_LIST: sendBtJammList();                 break; 
    case CMD_SET_JMMR_LIST: sendBtResponse(msg->cmdType, 1);  break;
    case CMD_GET_JMMR_DATA: sendBtJmmrData(msg);              break;
	}
	msg->cmdType = 0; 
}
 
 









    
 

 