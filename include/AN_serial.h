#ifndef _AN_SERIAL_
#define _AN_SERIAL_

#include "main.h"
#include "AN_serialConv.h"


class AN_serial{
private:

 
  std::string getCrcString(std::string strIn);
  std::string getDataStr(std::string strIn);

  int checkCrc(std::string crcStr, std::string data);
 
 

public:
  AN_serial();
  ~AN_serial();
  
  std::string dataPackStr;
  int  receiveDataPacks       = 0; 
  int  waitTimer              = 0;
  int  dataSrc                = 0;


 

  void resetDataPackProcess();

  void processingReceivedData();
  int  processingExternalData(std::string str);

  void checkPauseControl();

  void processingSerialData(_SERIAL_PACK dataPack);
 

};

 



#endif