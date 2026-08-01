 
#include "AN_taskRmReceive.h"
#include "main.h" 
 
TimerHandle_t xTimerRm;
AN_taskRmReceive::AN_taskRmReceive(/* args */){}
AN_taskRmReceive::~AN_taskRmReceive(){}



static bool timerIsStart = 0;
void rmTimerCallback(TimerHandle_t xTimerRm){
  _SERIAL_PACK sPack;
  configASSERT(xTimerRm);
  sPack.len = Serial1.available();
  sPack.data = static_cast<char *>(malloc(sPack.len));
  if(sPack.data == NULL) {
    ESP_LOGE("UART", "Malloc failed");
    return;
  }    
  Serial1.read(sPack.data, sPack.len);
  Serial.println(String(sPack.data));
  timerIsStart = 0;
  AN_taskRmReceive::getDevInfo(String(sPack.data));  
  free(sPack.data);  
  vTaskResume(Handle_taskRmReceive);   
}

void AN_taskRmReceive::initTimer(){
   xTimerRm = xTimerCreate("rmTimer", 30, pdFALSE, (void * ) 0, rmTimerCallback);
}

void startRmTimer(){
  // if(xTimerRm == NULL)return;
  xTimerStart(xTimerRm, 20);
}

void AN_taskRmReceive::fillDevParams(int dataArrLen, String *data){
    int tmpCnt = 0;
 
    String paramStr = "";
 
    for(int i=0; i<dataArrLen; i++){
        if(data[i].indexOf(":")== -1)continue;

        if(data[i].indexOf("MT"  )  != -1){G_lJmrStt.rebMod[G_selRm].mc    = data[i].substring(3, data[i].length()).toInt();  }
        if(data[i].indexOf("SP"  )  != -1){G_lJmrStt.rebMod[G_selRm].mask  = data[i].substring(3, data[i].length()).toInt();  }
        if(data[i].indexOf("VCPU")  != -1){G_lJmrStt.rebMod[G_selRm].vcpu  = data[i].substring(5, data[i].length()).toFloat();}
        if(data[i].indexOf("TEMP")  != -1){G_lJmrStt.rebMod[G_selRm].temp  = data[i].substring(5, data[i].length()).toFloat();}
    }      
}

void AN_taskRmReceive::getDevInfo(String data){
     
    String strArr[24] = {""};
    String delimiter = "\r\n";
    int start = 0;
    int end = data.indexOf(delimiter);
    int strCnt = 0;
 
    String tmpStr = "";
    // Serial.println("getDevInfo 52");
    while (end != -1) {
        strArr[strCnt] = data.substring(start, end);
        start = end + delimiter.length();
        end = data.indexOf(delimiter, start);
        strCnt++;
    }
    memset(G_lJmrStt.info, 0, TXT_BUFF_LEN);
    if((data.indexOf("ATI")) != -1){
        memccpy(G_lJmrStt.info, data.c_str(), '\0', data.length());
        G_lJmrStt.infoLen = data.length();
    }
    else  fillDevParams(strCnt, strArr); 
}
 
void AN_taskRmReceive::callback(){
  if(timerIsStart){
    xTimerReset(xTimerRm, 30);
  }else{
    startRmTimer();
    timerIsStart = 1;
  }
}

typedef struct {
    BYTE rx;
    BYTE tx;
}_RM_PINS;
 
_RM_PINS rmSer1;
_RM_PINS rmSer2;
_RM_PINS rmRxTx; 


static BYTE activeRebMod;
static int rmSel = 0;
static int rmNum = 0;


void AN_taskRmReceive::send(String str){
   
    static BYTE activeRebMod = 0; 
    if(activeRebMod != rmSel){
        activeRebMod = rmSel;
        rmRxTx = (activeRebMod == 0) ? rmSer1 : rmSer2;
        if(!activeRebMod) Serial1.begin(9600, SERIAL_8N1, UART_RM_RX1, UART_RM_TX1); 
        else              Serial1.begin(9600, SERIAL_8N1, UART_RM_RX2, UART_RM_TX2); 
    }    
 
    Serial.println("rx-> "+String(rmRxTx.rx)+" tx-> "+String(rmRxTx.tx));
    vTaskDelay(5);
    Serial1.println(str);
    Serial. println(str);
}

void AN_taskRmReceive::run(void *param){
  _SERIAL_PACK sPack;
  AN_shiftDataArr sft;
  _RM_AUT rmAut;
  int code;
  rmSer1.rx = 34;
  rmSer1.tx = 32;
  rmSer2.rx = 35;
  rmSer2.tx = 33;
  initTimer();
  for(;;){
    xQueueReceive(QueueRmEvent, &rmAut, (TickType_t)portMAX_DELAY);
    Serial.println("------ QueueRmEvent  ---------"); 
    if(rmAut.swtchActDev)G_selRm = 0;
    if(rmAut.rmSel)rmSel = rmAut.rmSel-1;
    for(int i=0; i<rmAut.opCodeQty; i++){
      Serial.println("------ stt 1 ---------"); 
      timerIsStart = 0;
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
      Serial.println("------ stt 2 ---------");                    
      vTaskSuspend(NULL);
      if(rmAut.swtchActDev)rmSel = (rmSel == 0) ? 1 : 0;
    }
  }
}

 



