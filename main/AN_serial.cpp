#include "../include/AN_serial.h"
#include "FastCRC.h"


AN_serial::AN_serial(){}
AN_serial::~AN_serial(){}

std::string AN_serial::getCrcString(std::string strIn){
  std::string crcStr;
  int startCrc = strIn.find("}_");
  int stopCrc  = strIn.rfind("_stop");
  
  if(startCrc == -1){
    crcStr = "-1";
    return crcStr;
  }
  if(stopCrc  == -1){
    crcStr = "-1";
    return crcStr;
  }
  
  int len = stopCrc - startCrc-2;
  crcStr = strIn.substr(startCrc+2, len);
  // Serial.println("crcStr -> "+ String(crcStr.c_str()));

  return crcStr;
}

std::string AN_serial::getDataStr(std::string strIn){
  int startData = strIn.find("start_");
  int stopData  = strIn.find("_stop");
  std::string result;
  if(startData == -1){
    result = "";
  }
  if(stopData  == -1){
    result = "";
  }
  
  int len = stopData - 8;
  result = strIn.substr(8, len);
  if(result.find("}_") != -1){
    result = result.substr(0, result.find("}_")+1);
  }


  return result;
}

int AN_serial::checkCrc(std::string crcExpStr, std::string data){
  FastCRC16 crc;
  int crcExp = 0;
  __try {
      crcExp = std::stoi(crcExpStr, nullptr, 16);
  } __catch (const std::invalid_argument&) {
      return 0; 
  } __catch (const std::out_of_range&) {
      return 0; 
  } 
 
  int len = data.length();
  int crc16 = crc.modbus((const uint8_t*) data.c_str(), len);

  // // Serial.println("crcExp -> "+String(crcExp, HEX)+" crc16 -> "+String(crc16, HEX));    
  if(crc16 == crcExp){
      return 1;
  }
  return 0;
}

void AN_serial::resetDataPackProcess(){
  waitTimer         = 0;
 
	receiveDataPacks  = 0;
	dataPackStr.clear();
}

void AN_serial::processingReceivedData(){
  _MSG_PACK msg;    
  AN_serialConv serialConv;
  
  if(dataPackStr.length() < 20){
    resetDataPackProcess();	
    return;           
  }

  std::string dataStr = getDataStr(dataPackStr);
  std::string crcStr =  getCrcString(dataPackStr);
  dataPackStr.clear();

 
  
  if(dataSrc == SERIAL_SRC_485){
    if(!checkCrc(crcStr, dataStr)){
      G_serialBusy = 0; 
      AN_print("error crc rs485");
      return;
    }
  } 

  
  if(serialConv.unpackData((char*)dataStr.c_str(), &msg)){ //there is error in JSON-data
      resetDataPackProcess();
  }else{
 
    waitTimer = 0;
    resetDataPackProcess();
    // AN_shiftDataArr sft;
    // sft.printMsg(&msg);
    if(msg.addrEsp32 == G_lJmrStt.esp32Addr){ 
        xQueueSend(QueueCmd, &msg, 100);
        G_serialBusy = 0; 
    }
  }		

	G_serialBusy = 0; 
}

int AN_serial::processingExternalData(std::string str){
 
	if(str.length()>120)dataPackStr.append(str.substr(0,120));
	else				        dataPackStr.append(str);
  
	if(dataPackStr.length() > MAX_SERIAL_DATA_LEN){
		resetDataPackProcess();
    return 0;
	}
 
  int found = dataPackStr.find("stop");
  if(found == -1)return 0;

  processingReceivedData();
  G_serialBusy = 0; 
  return 0;   	
}

void AN_serial::checkPauseControl(){
    if(dataSrc == SERIAL_SRC_485){
      if(G_wait485PackCnt > 8){       
        resetDataPackProcess();
      }
    }
    
    if(dataSrc == SERIAL_SRC_BT){
      if(G_waitBtPackCnt > 8){       
        resetDataPackProcess();
      }
    }  
}

void AN_serial::processingSerialData(_SERIAL_PACK dataPack)
{
    AN_shiftDataArr sft;
    _MSG_PACK msg;
    AN_serialConv  serialConv;
    
    checkPauseControl();
    char *data = static_cast<char *>(malloc(dataPack.len)); 
    if (data == NULL) {
        ESP_LOGE("SERIAL", "processingSerialData : Malloc failed");
        return;
    }
    memccpy(data, dataPack.data, 0, dataPack.len);
    std::string start = "start";
    waitTimer = 0;
    if(std::string(data).substr(0, start.length()) == start){
        receiveDataPacks = 1;  
    }
    if(receiveDataPacks){
        processingExternalData(std::string(data));
    }else{
        if(serialConv.unpackData(data, &msg)){ //there is error in JSON-data
            AN_print("Error JSON data");
            G_serialBusy = 0; 
        }else{
            G_serialBusy = 0; 
            msg.direction = MSG_DIR_REQUEST;
            xQueueSend(QueueCmd, &msg, 100);
        }
    }
    free(data);
}
