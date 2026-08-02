#ifndef _AN_SERIAL_CONV_
#define _AN_SERIAL_CONV_

#include "stdio.h"
#include <string>

#include "AN_objects.h"
#include "define.h"
#include "AN_shiftDataArr.h"

class AN_serialConv{
private:

public:
  AN_serialConv(/* args */);
  ~AN_serialConv();
 

  int getJmmrList(char *data);

  int findParam(char *data, char *param, DWORD *paramVal);

  int unpackData(char *data, _MSG_PACK *msg);

  int saveMsgParam(char *param, char *val, _MSG_PACK *msg);
 

  int saveJmmrParam(char *param, char *val, _JMMR_STATE *jmmr);

 
  int getParam(std::string *str, char *param, char *val);

  int deserializeDataPack(_JMMR_STATE *jmmr, _MSG_PACK *msg, char *data);

  int serializeRs485Data(_MSG_PACK *msg, char *data);

  int serializeMsgData(_MSG_PACK *msg, char *data);

  int serializeJmmrData(_JMMR_STATE *jmmr, char *data);

  int serializeJmmrList(std::vector<_JMMR_STATE> jmmrsList, char *data);

 
};



#endif