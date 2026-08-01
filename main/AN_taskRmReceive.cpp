#include "../include/AN_taskRmReceive.h"
#include "AN_taskRmReceive.h"
#include "AN_taskRmReceive.h"
 

AN_taskRmReceive::AN_taskRmReceive(/* args */){}
AN_taskRmReceive::~AN_taskRmReceive(){}

static int rmNum;
void AN_taskRmReceive::fillDevParams(int dataArrLen, String *data){
    int tmpCnt = 0;
 
    String paramStr = "";
 
    for(int i=0; i<dataArrLen; i++){
        if(data[i].indexOf(":")== -1)continue;

        if(data[i].indexOf("MT"  )  != -1){G_lJmrStt.rebMod[rmNum].mc    = data[i].substring(3, data[i].length()).toInt();  }
        if(data[i].indexOf("SP"  )  != -1){G_lJmrStt.rebMod[rmNum].mask  = data[i].substring(3, data[i].length()).toInt();  }
        if(data[i].indexOf("VCPU")  != -1){G_lJmrStt.rebMod[rmNum].vcpu  = data[i].substring(5, data[i].length()).toFloat();}
        if(data[i].indexOf("TEMP")  != -1){G_lJmrStt.rebMod[rmNum].temp  = data[i].substring(5, data[i].length()).toFloat();}
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
  _SERIAL_PACK sPack;
  int len = Serial1.available();
  sPack.data = static_cast<char *>(malloc(len));
  if(sPack.data == NULL) {
        ESP_LOGE("UART", "Malloc failed");
    return;
  }
  Serial1.read(sPack.data, len);
  sPack.len = len;
  sPack.dir = MSG_DIR_RESPONSE;
  xQueueSend(QueueRmEvent, &sPack, portMAX_DELAY);
}

void AN_taskRmReceive::run(void *param){
  _SERIAL_PACK sPack;
  for(;;){
    if (xQueueReceive(QueueRmEvent, &sPack, (TickType_t)portMAX_DELAY)) {
        if(sPack.dir == MSG_DIR_RESPONSE){

        }else{

        }
        // Serial.println(" - - data from RM - - -");
        Serial.write(sPack.data, sPack.len);
        free(sPack.data);
      
    }
  }
}





