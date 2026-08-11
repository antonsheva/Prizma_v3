 
#include "AN_taskRmAut.h"
 
 
TimerHandle_t xTimerRm;
AN_taskRmAut::AN_taskRmAut(/* args */){}
AN_taskRmAut::~AN_taskRmAut(){}
 
 static BYTE activeRebMod;
static int rmSel = 0;
 

void startRmTimer(){
  xTimerStart(xTimerRm, 20);
}

void AN_taskRmAut::fillDevParams(int dataArrLen, String *data){ 
    for(int i=0; i<dataArrLen; i++){
        if(data[i].indexOf(":")== -1)continue;
        if(data[i].indexOf("MT"  )  != -1){
          G_lJmrStt.rebMod[rmSel].mc    = data[i].substring(3, data[i].length()).toInt();  
          continue;
        }
        if(data[i].indexOf("SP"  )  != -1){
          G_lJmrStt.rebMod[rmSel].mask  = data[i].substring(3, data[i].length()).toInt();  
          continue;     
        }
        if(data[i].indexOf("VCPU")  != -1){
          G_lJmrStt.rebMod[rmSel].vcpu  = data[i].substring(5, data[i].length()).toFloat();
          continue;
        }
        if(data[i].indexOf("TEMP")  != -1){
          G_lJmrStt.rebMod[rmSel].temp  = data[i].substring(5, data[i].length()).toFloat();      
          continue;
        }
    }      
}

void AN_taskRmAut::getDevInfo(String data){
     
    String strArr[24] = {""};
    String delimiter = "\r\n";
    int start = 0;
    int end = data.indexOf(delimiter);
    int strCnt = 0;
 
    String tmpStr = "";
    while (end != -1) {
        strArr[strCnt] = data.substring(start, end);
        start = end + delimiter.length();
        end = data.indexOf(delimiter, start);
        strCnt++;
    }
    memset(G_lJmrStt.info, 0, TXT_INFO_LEN);
    if((data.indexOf("ATI")) != -1){
        memccpy(G_lJmrStt.info, data.c_str(), '\0', data.length());
        G_lJmrStt.infoLen = data.length();
        G_lJmrStt.rebMod[rmSel].infoLen =  data.length(); 
        memccpy(G_lJmrStt.rebMod[rmSel].info, data.c_str(), '\0', data.length());
    }
    else  
    fillDevParams(strCnt, strArr); 
}
 
 

void AN_taskRmAut::readData(){
  
  _SERIAL_PACK sPack;
  sPack.len = Serial1.available();
  sPack.data = static_cast<char *>(malloc(sPack.len));
  if(sPack.data == NULL) {
    ESP_LOGE("UART", "Malloc failed");
    return;
  }    
  memset(sPack.data, 0, sPack.len);
  Serial1.read(sPack.data, sPack.len);
  AN_taskRmAut::getDevInfo(String(sPack.data));  
  free(sPack.data);  
}

void AN_taskRmAut::callback(){
    G_pauseRmDataCnt = 10;
}

void AN_taskRmAut::send(String str){
    if(activeRebMod != rmSel){
        activeRebMod = rmSel;
        if(!activeRebMod) Serial1.begin(9600, SERIAL_8N1, UART_RM_RX1, UART_RM_TX1); 
        else              Serial1.begin(9600, SERIAL_8N1, UART_RM_RX2, UART_RM_TX2); 
    }    
    vTaskDelay(5);
    Serial1.println(str);
    Serial.println(str);    
}

void AN_taskRmAut::run(void *param){
  _SERIAL_PACK sPack;
  AN_shiftDataArr sft;
  _RM_AUT rmAut;
  int code;

  for(;;){
    xQueueReceive(QueueRmEvent, &rmAut, (TickType_t)portMAX_DELAY);
    if(rmAut.swtchActDev)rmSel = 0;
    if(rmAut.rmSel)rmSel = rmAut.rmSel-1;
    for(int i=0; i<rmAut.opCodeQty; i++){ 
      code = rmAut.opCodeList[i];
      if(code == CMD_RM_AT       )  send("AT\n\r");   
      if(code == CMD_RM_GET_ATBT )  send("ATBT\n\r"); 
      if(code == CMD_RM_GET_ATC  )  send("ATC\n\r");  
      if(code == CMD_RM_GET_ATI  )  send("ATI\n\r");  
      if(code == CMD_RM_ATZ      )  send("ATZ\n\r");  
      if(code == CMD_RM_SET_ATW  )  send("AT&W\n\r");       
      if(code == CMD_RM_SET_ATC  ){
          String str = "ATC="+String(G_lJmrStt.rebMod[rmSel].mc)+
                                    ","+String(G_lJmrStt.rebMod[rmSel].mask)+"\n\r"; 
                                    send(str);   

      }
      G_rebModAut_tm = 0;
      vTaskSuspend(NULL);    
      readData();
      if(rmAut.swtchActDev)rmSel = (rmSel == 0) ? 1 : 0;
    }
    sft.printJmmrData(&G_lJmrStt);
    if(rmAut.cmd == CMD_RESTART_ESP){
      sPack.cmd = EVENT_RESTART_ESP;
      xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY); 
    }
    if(rmAut.cmd == CMD_RESUME_WORK){
      sPack.cmd = EVENT_RESUME_WORK;
      xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY); 
    }    
    
  }
}

 



