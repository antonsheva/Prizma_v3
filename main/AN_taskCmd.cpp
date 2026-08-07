 
#include "AN_taskCmd.h"


#define MAX_JMMR_QTY 10

 
void AN_taskCmd::init()
{
  
}
int AN_taskCmd::processingResponseData(_MSG_PACK *msg){
  AN_shiftDataArr sft;
  AN_commRs485Bt        btSend;
  switch (msg->response){
		case RESP_GET_JMMR_LIST: addJmmr(msg); break;
		case RESP_GET_JMMR_DATA: btSend.sendBtJmmrData(msg);
			msg->addrEsp32 = msg->sender;	
			xQueueSend(QueueBt, msg, portMAX_DELAY);
			_JMMR_STATE jmmr;
			sft.loadMsgToJmrStt(msg, &jmmr);
			sft.printJmmrData(&jmmr);
			break;
	}
	G_serialBusy = 0; 
	return 0 ;
}

void AN_taskCmd::processingCmd(_MSG_PACK *msg){
	switch (msg->cmd){
		case CMD_RM_AT			 			: sendCmdToRm(CMD_RM_AT,         msg->mask1);	break;
		case CMD_RM_GET_ATBT 			: sendCmdToRm(CMD_RM_GET_ATBT,	 	msg->mask1);	break;
		case CMD_RM_GET_ATC	 			: sendCmdToRm(CMD_RM_GET_ATC,		msg->mask1);	break;
		case CMD_RM_GET_ATI	 			: sendCmdToRm(CMD_RM_GET_ATI,		msg->mask1);	break;
		case CMD_RM_GET_STATE			: rmGetState();  															break;

		case CMD_PRINT_JMMR_DATA  : printJmmrData(msg); 												break;
		/**
		 * @brief preferences commands
		 * saving data to preference such as: addresses, ID, group...
		 */
		case CMD_SET_ADDR_ESP   : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break;
		case CMD_SET_ADDR_RM    : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		case CMD_SET_PWR        : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		case CMD_SET_DEV_ID     : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		case CMD_SET_GROUP_ID   : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		case CMD_SET_DEV_TYPE   : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		case CMD_SET_DEV_RANGE  : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		case CMD_GET_DEV_PARAM  : xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		case CMD_PRINT_ADDRESSES: xQueueSend(QueuePrefs, msg, portMAX_DELAY); break; 
		/**
		 * @brief  RS485-related commands
		 * 
		 */
		case CMD_GET_JMMR_LIST 	: getJammList();  			break;
		case CMD_SET_JMMR_LIST 	: setJmmrList();				break;
		case CMD_GET_JMMR_DATA 	: getJmmrData(msg);  	  break; 
		case CMD_SET_JMMR_DATA 	: setJmmrData(msg);     break;
 
  	/**
		 * @brief BT functions
		 * 
		 */
		case CMD_BT_START       : btInit();							break;     
		case CMD_BT_STOP        : btStop();							break;   

		/**
		 * @brief other funcs
		 * 
		 */
		case CMD_APLAY_PWR      : applyPwr();						break;
		case CMD_UPDT_LOC_DATA  : updateLocalData(msg);	break;
		case CMD_GET_ALL_STACK  : getAllStack();				break;
			
		case CMD_TEST           : test(); 							break;
 
	}
	G_serialBusy = 0;
}

 
void AN_taskCmd::printJmmrList(){
	AN_shiftDataArr sft;
	sft.printJmmrList();
}
 

void AN_taskCmd::printJmmrData(_MSG_PACK *msg){
	AN_commRs485Bt comm;
  AN_shiftDataArr sft;
	if(msg->addrEsp32 == 0){
		sft.printJmmrData(&G_lJmrStt);
		return;
	}
	msg->cmdType = CMD_GET_JMMR_DATA;
	msg->addressee = msg->addrEsp32;
	comm.prepMsg(msg,0);
  xQueueSend(QueueRs485Send, msg, portMAX_DELAY);	
} 

 

int AN_taskCmd::getJmmrData(_MSG_PACK *msg){
	AN_shiftDataArr sft;
	msg->addressee  = msg->sender;
	msg->direction  = MSG_DIR_RESPONSE;		
	if(msg->response == RESP_GET_JMMR_DATA){
		memset(msg->txt, 0, TXT_INFO_LEN);
		memcpy(msg->txt, G_lJmrStt.info, G_lJmrStt.infoLen);
		msg->txtLen = G_lJmrStt.infoLen;
	}
	sft.loadJmmrStateToMsg(msg, &G_lJmrStt);
	xQueueSend(QueueRs485Send, msg, portMAX_DELAY);
	return 0;   
} 

void AN_taskCmd::rmGetState(){
	_RM_AUT rmAut;
	rmAut.opCodeList[0] = CMD_RM_GET_ATI ;
	rmAut.opCodeList[1] = CMD_RM_GET_ATI ;
	rmAut.opCodeList[2] = CMD_RM_GET_ATC ;
	rmAut.opCodeList[3] = CMD_RM_GET_ATC ;
	rmAut.opCodeList[4] = CMD_RM_GET_ATBT;
	rmAut.opCodeList[5] = CMD_RM_GET_ATBT;
	rmAut.opCodeQty   = 6;
	rmAut.swtchActDev = true;
	xQueueSend(QueueRmEvent, &rmAut, portMAX_DELAY); 
}

void AN_taskCmd::setJmmrData(_MSG_PACK *msg){
	AN_shiftDataArr sft;
	 
	sft.printMsg(msg);
	if(msg->updtLocalJmmr)sft.loadMsgToJmrStt(msg, &G_lJmrStt, 1);

	_SERIAL_PACK sPack;
	sPack.cmd = EVENT_APPLY_CHANGES;
	      
	_RM_AUT rmAut;	
	rmAut.opCodeList[0] = CMD_RM_SET_ATC ;
	rmAut.opCodeList[1] = CMD_RM_SET_ATC ;
  rmAut.opCodeList[2] = CMD_RM_SET_ATW ;
	rmAut.opCodeList[3] = CMD_RM_SET_ATW ;
	rmAut.opCodeList[4] = CMD_RM_ATZ     ;
	rmAut.opCodeList[5] = CMD_RM_ATZ     ;
	rmAut.opCodeList[6] = CMD_RM_GET_ATI ;
	
	rmAut.opCodeQty = 7;
	rmAut.swtchActDev = true;
 	rmAut.cmd = CMD_RESTART_ESP;

  msg->cmd = CMD_SET_PWR;

	if(msg->needBtOff) btStop(); 
	applyPwr();
	xQueueSend(QueuePrefs, msg, portMAX_DELAY);
  xQueueSend(QueueRmEvent, &rmAut, portMAX_DELAY);
	xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY); 
	// cRebMod->cmdAfterAutFinish = CMD_RESTART_ESP;
 
}

void AN_taskCmd::test(){
	_SERIAL_PACK sPack;
	char data[] = "ATI\n\r"; 
	sPack.data = static_cast<char*>(malloc(sizeof(data)));
	memccpy(sPack.data, data, 0, sizeof(data));
	sPack.len = sizeof(data);
  selectRmModule(UART_SELECT_RM1);
  
}

void AN_taskCmd::btInit(){
	_SERIAL_PACK sPack;
	AN_bt bt;
	bt.init();
}

void AN_taskCmd::btStop(){
	AN_bt bt;
	bt.end();  
}

void AN_taskCmd::addJmmr(_MSG_PACK *msg){
	_JMMR_STATE jmmr; 
  AN_shiftDataArr sft;
	jmmr.esp32Addr = msg->sender;
	sft.loadMsgToJmrStt(msg, &jmmr);
	G_jmmrsList.push_back(jmmr);
}

void AN_taskCmd::addJmmr(_JMMR_STATE *jmmr){
	_JMMR_STATE j; 
  AN_shiftDataArr sft;
	sft.copyJmmr(&j, jmmr); 
	G_jmmrsList.push_back(j);
}

void AN_taskCmd::setJmmrList(){
	_MSG_PACK msg;
	msg.cmdType = CMD_SET_JMMR_LIST;
  msg.subscribersQty = G_jmmrsList.size(); 	
	xQueueSend(QueueRs485Pool, &msg, portMAX_DELAY);
}

void AN_taskCmd::updateLocalData(_MSG_PACK *msg)
{
  AN_shiftDataArr sft;
	if(msg->cmdType == CMD_GET_JMMR_LIST){
    addJmmr(&G_lJmrStt);
	}
	if(msg->cmdType == CMD_SET_JMMR_LIST){
		sft.loadMsgToJmrStt(msg, &G_lJmrStt); 
		// ASetExpectedEvent(Event_finishLoadConfig);
	}
}

void AN_taskCmd::getAllStack(){
	_MSG_PACK msg;
	for(int i=1; i<=12; i++){
		msg.mask1 = i;
		getStackSize(&msg);
		vTaskDelay(10/portTICK_PERIOD_MS);
	}
}

void AN_taskCmd::getStackSize(_MSG_PACK *msg){
	// TaskHandle_t tHandle;
	// UBaseType_t uxHighWaterMark;
	/* Print actual size of stack has used */
 
	// switch(msg->mask1){

		// case  1  : tHandle = TaskHandle_usb         ; break; 
		// case  2  : tHandle = TaskHandle_cmd         ; break; 
		// case  3  : tHandle = TaskHandle_prefs       ; break; 
		// case  4  : tHandle = TaskHandle_Rs485Poll   ; break; 
		// case  5  : tHandle = TaskHandle_Rs485Send   ; break; 
		// case  6  : tHandle = TaskHandle_Rs485Receive  ; break; 
		// case  7  : tHandle = TaskHandle_PwrAut        ; break; 
		// case  8  : tHandle = TaskHandle_Button        ; break; 
		// case  9  : tHandle = TaskHandle_Leds          ; break;  
		// case  10 : tHandle = TaskHandle_Monitor       ; break; 
		// case  11 : tHandle = TaskHandle_AnalogParam   ; break; 
		// case  12 : tHandle = TaskHandle_BtSend        ; break; 
		// case  13 : tHandle =           ; break;     
		// case  14 : tHandle =           ; break; 
		// case  13 : tHandle =           ; break;     
		// case  14 : tHandle =           ; break; 
		// case  13 : tHandle =           ; break;     
 

	// }

  // uxHighWaterMark = uxTaskGetStackHighWaterMark(tHandle);
	// AN_print("---------------------"); 
	// AN_print("Task "+std::string(msg->mask1)+ " - > "+std::string(uxHighWaterMark));
 
}

int AN_taskCmd::getJammList(){
	_MSG_PACK msg;
	G_subscribersQty = MAX_DEVICE_QTY;
	G_jmmrsList.clear();
  addJmmr(&G_lJmrStt);
	msg.subscribersQty = MAX_DEVICE_QTY;
	msg.cmdType = CMD_GET_JMMR_LIST;
	xQueueSend(QueueRs485Pool, &msg, portMAX_DELAY);	
  return 0;
}

void AN_taskCmd::applyPwr(){
  if(G_lJmrStt.rebMod[0].pwr == 1)	JMMR_1_ON
  else								              JMMR_1_OFF

  if(G_lJmrStt.rebMod[1].pwr == 1)	JMMR_2_ON
  else								              JMMR_2_OFF

}

void AN_taskCmd::run(void *param){
  _MSG_PACK msg;
  selectRmModule(UART_SELECT_RM1, 1);
  for(;;){
    xQueueReceive(QueueCmd, &msg, portMAX_DELAY);
    if(msg.direction == MSG_DIR_RESPONSE){
      processingResponseData(&msg);
    }else{
      processingCmd(&msg);
    } 
  }
}

void AN_taskCmd::sendCmdToRm(int cmd, int sel){
 
	vTaskDelay(10/portTICK_PERIOD_MS);
	switch (cmd)	{
		case CMD_RM_AT       : Serial1.print ("AT\r\n");  break;            
		case CMD_RM_GET_ATBT : Serial1.print ("ATBT\r\n");  break;            
		case CMD_RM_GET_ATC  : Serial1.print ("ATC\r\n");  break;            
		case CMD_RM_SET_ATC  : Serial1.print ("ATC\r\n");  break;   
		case CMD_RM_SET_ATE0 : Serial1.print ("ATE0\r\n");  break;             
		case CMD_RM_SET_ATE1 : Serial1.print ("ATE1\r\n");  break;             
		case CMD_RM_GET_ATI  : Serial1.print ("ATI\r\n");  break;            
		case CMD_RM_ATZ      : Serial1.print ("ATZ\r\n");  break;            
		case CMD_RM_GET_ATW  : Serial1.print (" \n\r");  break;            
		case CMD_RM_SET_ATW  : Serial1.print ("AT&W\n\r");  break;             
		// case CMD_GET_INFO    : Serial1.print (" \n\r");  break;
	}
   
 
}

void AN_taskCmd::selectRmModule(int sel, bool firstInit){
 
 
 
	 
}



AN_taskCmd::AN_taskCmd(/* args */){}
AN_taskCmd::~AN_taskCmd(){}














