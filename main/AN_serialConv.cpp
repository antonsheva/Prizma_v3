#include "AN_serialConv.h"

 
AN_serialConv::AN_serialConv(/* args */){
 
}

AN_serialConv::~AN_serialConv()
{
}

 
int AN_serialConv::getJmmrList(char * data){
  char substr[TXT_BUFF_LEN] = {0};
  DWORD posEnd;
  DWORD posStart = std::string(data).find("[");
  DWORD cnt = 0;
  DWORD len = 0;
  int localCnt;

  bool  endOfList = 0;
  if(posStart != -1){
    posEnd = std::string(data).find("]");
    if(posEnd != -1){
      len = posEnd - posStart;
      G_jmmrsList.clear();
      while(1){
        _JMMR_STATE jmmr;
        memset(substr, 0, TXT_BUFF_LEN);
        for(localCnt=0; localCnt< TXT_BUFF_LEN; localCnt++){
          if( data[localCnt+posStart+cnt] == '}')break;
          if( data[localCnt+posStart+cnt] == ']'){
            endOfList = 1; 
            break;
            
          }
          substr[localCnt] = data[localCnt+posStart+1+cnt];
          if(cnt + localCnt > (len-1)){
            endOfList = 1; 
            break;
          }
        }
        if(endOfList)break;
        cnt+= localCnt+1;
        deserializeDataPack(&jmmr, NULL, substr);
        G_jmmrsList.push_back(jmmr);     
      }
    }
  }
  return 0;
}

int  AN_serialConv::findParam(char *data, char *param, DWORD *paramVal){
  std::string substr;
  int pos = std::string(data).find(std::string(param));
  int pos1;
  char strParam[MAX_STR_LEN];
  char strVal  [MAX_STR_LEN];
  int result = 0;
  if(pos != -1){
    pos1 = std::string(data).find(",", pos);
    if(pos1 == -1)pos1 = std::string(data).find("}", pos);
    if(pos != -1){
      substr = std::string(data).substr(pos, pos1-pos);
      getParam(&substr, strParam, strVal);
      *paramVal = atoi(strVal);
      // AN_print("param -> "+std::string(param)+" : val -> "+std::string(strVal) );
      result = 1;
    }
  }
  return result;
}

int AN_serialConv::unpackData(char *data, _MSG_PACK *msg){
  std::string substr;
  AN_shiftDataArr sft;

  char paramCmd[]     = PARAM_CMD;
  char paramSender[]  = PARAM_SENDER;
  char paramAdEsp[]   = PARAM_ADDR_ESP; 
  char paramListLen[] = PARAM_JMMR_LIST_LEN;    
  DWORD cmd     =  0;
  DWORD sender  =  0;
  DWORD adEsp   =  0; 
  DWORD listLen =  0;    

  findParam(data,  paramCmd , &cmd);
  if(cmd == CMD_SET_JMMR_LIST){
    findParam(data, paramSender, &sender);
    findParam(data, paramAdEsp, &adEsp);
    findParam(data, paramListLen, &listLen);
    
    
    getJmmrList(data);
    msg->sender     = sender;
    msg->cmd        = cmd;
    msg->addrEsp32  = adEsp;
    msg->jmmrListLen= listLen;

  }else{
    deserializeDataPack(NULL, msg, data);
  }
  return 0;
}

int AN_serialConv::saveMsgParam(char *param, char *val, _MSG_PACK *msg){
  // std::string str = "param -> "+std::string(param)+" : val -> "+std::string(val);
 
  std::string par = std::string(param); 
  if(par.find(PARAM_CMD       )   != -1)msg->cmd        = atoi(val); 
  if(par.find(PARAM_SENDER    )   != -1)msg->sender     = atoi(val); 
  if(par.find(PARAM_ADDRESSEE )   != -1)msg->addressee  = atoi(val);   
  if(par.find(PARAM_RESPONSE  )   != -1)msg->response   = atoi(val); 
  if(par.find(PARAM_MSG_DIR   )   != -1)msg->direction  = atoi(val); 
  if(par.find(PARAM_DEV_ID    )   != -1)msg->devId      = atoll(val);                  
  if(par.find(PARAM_GROUP_ID  )   != -1)msg->groupId    = atoi(val);                               
  if(par.find(PARAM_DEV_TYPE  )   != -1)msg->devType    = atoi(val);              
  if(par.find(PARAM_DEV_RANGE )   != -1)msg->devRange   = atoi(val);                
  if(par.find(PARAM_ADDR_ESP  )   != -1)msg->addrEsp32  = atoi(val);        
  if(par.find(PARAM_ADDR_RM_1 )   != -1)msg->addrRm1    = atoi(val);        
  if(par.find(PARAM_ADDR_RM_2 )   != -1)msg->addrRm2    = atoi(val);        
  if(par.find(PARAM_MOD_CODE_1)   != -1)msg->modCode1   = atoi(val);     
  if(par.find(PARAM_MOD_CODE_2)   != -1)msg->modCode2   = atoi(val);     
  if(par.find(PARAM_MASK_1    )   != -1)msg->mask1      = atoi(val);     
  if(par.find(PARAM_MASK_2    )   != -1)msg->mask2      = atoi(val);     
  if(par.find(PARAM_PWR_1     )   != -1)msg->pwr1       = atoi(val);  
  if(par.find(PARAM_PWR_2     )   != -1)msg->pwr2       = atoi(val);   
  if(par.find(PARAM_RM_NUM    )   != -1)msg->rmNum      = atoi(val);  
  if(par.find(PARAM_BATT_STATE)   != -1)msg->devBattStt = atoi(val);
  if(par.find(PARAM_TEMPERATURE)  != -1)msg->devTemper  = atoi(val);
  if(par.find(PARAM_NEED_BT_OFF)  != -1)msg->needBtOff  = atoi(val);       
  
  
  if(par.find(PARAM_JMMR_LIST_LEN) != -1)msg->jmmrListLen = atoi(val);   

  if(par.find(PARAM_TXT_LEN   ) != -1)msg->txtLen    = atoi(val); 
  else if(par.find(PARAM_TXT  ) != -1)memccpy(msg->txt, val, 0, TXT_BUFF_LEN);
  return 0;
}

int AN_serialConv::saveJmmrParam(char *param, char *val, _JMMR_STATE *jmmr){
  // std::string str = "param -> "+std::string(param)+" : val -> "+std::string(val);
 
  std::string par = std::string(param); 
  if(par.find(PARAM_DEV_ID    )   != -1)jmmr->devId               = atoll(val);                  
  if(par.find(PARAM_GROUP_ID  )   != -1)jmmr->groupId             = atoi(val);                               
  if(par.find(PARAM_DEV_TYPE  )   != -1)jmmr->devType             = atoi(val);              
  if(par.find(PARAM_DEV_RANGE )   != -1)jmmr->devRange            = atoi(val);                
  if(par.find(PARAM_ADDR_ESP  )   != -1)jmmr->esp32Addr           = atoi(val);        
  if(par.find(PARAM_ADDR_RM_1 )   != -1)jmmr->rebMod[0].address   = atoi(val);        
  if(par.find(PARAM_ADDR_RM_2 )   != -1)jmmr->rebMod[1].address   = atoi(val);        
  if(par.find(PARAM_MOD_CODE_1)   != -1)jmmr->rebMod[0].mc        = atoi(val);     
  if(par.find(PARAM_MOD_CODE_2)   != -1)jmmr->rebMod[1].mc        = atoi(val);     
  if(par.find(PARAM_MASK_1    )   != -1)jmmr->rebMod[0].mask      = atoi(val);     
  if(par.find(PARAM_MASK_2    )   != -1)jmmr->rebMod[1].mask      = atoi(val);     
  if(par.find(PARAM_PWR_1     )   != -1)jmmr->rebMod[0].pwr       = atoi(val);  
  if(par.find(PARAM_PWR_2     )   != -1)jmmr->rebMod[1].pwr       = atoi(val);  
  if(par.find(PARAM_BATT_STATE)   != -1)jmmr->devBattStt          = atoi(val);
  if(par.find(PARAM_TEMPERATURE)  != -1)jmmr->devTemper           = atoi(val); 
  
  if(par.find(PARAM_TXT_LEN   ) != -1)jmmr->infoLen            = atoi(val); 
  else if(par.find(PARAM_TXT  ) != -1)memccpy(jmmr->info, val, 0, 1024);
  return 0;
}


int AN_serialConv::getParam(std::string *str, char *param, char *val){
  std::string subs1;
  std::string subs2;
  std::string subs3;
  char buff[TXT_BUFF_LEN] = {0};
  int cnt = 0;
  for(int i=0; i<TXT_BUFF_LEN; i++){
    if(str->c_str()[i] != '\"'){
      buff[cnt] = str->c_str()[i];
      cnt++;
    }
    if(str->c_str()[i] == '\0')break;
  }
        
  subs3 = std::string(buff);
  int pos1;
  pos1 = subs3.find(":");
  if(pos1 != -1){
    subs1 = subs3.substr(0,pos1);
    subs2 = subs3.substr(++pos1);
    memccpy(param, subs1.c_str(), 0, MAX_STR_LEN);
    memccpy(val,   subs2.c_str(), 0, TXT_BUFF_LEN);
  }
  return 0;
}



int AN_serialConv::deserializeDataPack(_JMMR_STATE *jmmr, _MSG_PACK *msg, char *data){
  char strBuff  [TXT_BUFF_LEN];
  char paramBuff[MAX_STR_LEN ];
  char valBuff  [TXT_BUFF_LEN];
   
  memccpy(strBuff, data,0,TXT_BUFF_LEN);
  memset(paramBuff, 0, MAX_STR_LEN );
  memset(valBuff,   0, TXT_BUFF_LEN);  

  std::string str = std::string(strBuff);
  std::string substr;
  std::string subs1;
  std::string subs2;
  std::string subs3;  
   
  int pos = str.find("{");
  if(pos != -1)str.erase(pos,1);
  pos = str.find("}");
  if(pos != -1)str.erase(pos,1);
  
  for(int i=0; i<32; i++){
    pos = str.find(",");
    if(pos != -1){
      substr = str.substr(0,pos++);
      str = str.substr(pos);
      getParam(&substr, paramBuff, valBuff);
      if(jmmr != NULL) saveJmmrParam(paramBuff, valBuff, jmmr);
      if(msg  != NULL) saveMsgParam(paramBuff, valBuff, msg);      
    }else {
      getParam(&str, paramBuff, valBuff);   
      if(jmmr != NULL) saveJmmrParam(paramBuff, valBuff, jmmr);
      if(msg  != NULL) saveMsgParam(paramBuff, valBuff, msg);
      break;
    }
    if(msg  != NULL){
      if(msg->jmmrListLen > 0){

      }
    }
  }
   

  return 0;
}


int AN_serialConv::serializeRs485Data(_MSG_PACK *msg, char *data){
  std::string str = "{";
  str.append("\"cmd\":"        +std::to_string(msg->cmd)+",");
  str.append("\"sender\":"     +std::to_string(G_lJmrStt.esp32Addr)+",");
  str.append("\"addressee\":"  +std::to_string(msg->addressee)+",");
  str.append("\"resp\":"       +std::to_string(msg->response)+",");
  str.append("\"dir\":"        +std::to_string(msg->direction)+",");
  str.append("\"ad_esp\":"     +std::to_string(msg->addrEsp32)+",");
  str.append("\"need_bt_off\":"+std::to_string(msg->needBtOff)+","); 
  
  
  if(msg->direction == MSG_DIR_RESPONSE){
    str.append("\"dev_id\":"    +std::to_string(msg->devId)+",");
    str.append("\"group_id\":"  +std::to_string(msg->groupId)+",");
    str.append("\"dev_type\":"  +std::to_string(msg->devType)+",");
    str.append("\"dev_range\":" +std::to_string(msg->devRange)+",");
    str.append("\"batt_stt\":"  +std::to_string(msg->devBattStt)+",");
    str.append("\"temper\":"    +std::to_string(msg->devTemper)+",");
  }

  if((msg->cmd == CMD_SET_JMMR_DATA)||(msg->direction == MSG_DIR_RESPONSE)){
    str.append("\"mc1\":"       +std::to_string(msg->modCode1)+",");
    str.append("\"mc2\":"       +std::to_string(msg->modCode2)+",");
    str.append("\"msk1\":"      +std::to_string(msg->mask1)+",");
    str.append("\"msk2\":"      +std::to_string(msg->mask2)+",");
    str.append("\"pwr1\":"      +std::to_string(msg->pwr1)+",");
    str.append("\"pwr2\":"      +std::to_string(msg->pwr2)+",");

    if(msg->addrRm1 && msg->addrRm1 < 128) str.append("\"ad_rm1\":"    +std::to_string(msg->addrRm1)+",");
	  if(msg->addrRm2 && msg->addrRm2 < 128) str.append("\"ad_rm2\":"    +std::to_string(msg->addrRm2)+",");
  }

  if((msg->response == RESP_GET_JMMR_DATA)&&(msg->direction == MSG_DIR_RESPONSE)){
    str.append("\"txt_len\":"   +std::to_string(msg->txtLen)+",");
    str.append("\"txt\": \""    +std::string(msg->txt)+"\"");
  }

  int pos = str.rfind(",");
  int len = str.length();
  if(pos == len-1)str.erase(pos, 1);

  str.append("}");
  memccpy(data, str.c_str(), 0, TXT_BUFF_LEN);
  return str.length();
}

int AN_serialConv::serializeMsgData(_MSG_PACK *msg, char *data){
  std::string str = "{";
  str.append("\"cmd\":"        +std::to_string(msg->cmd)+",");
  str.append("\"sender\":"     +std::to_string(msg->sender)+",");
  str.append("\"resp\":"       +std::to_string(msg->response)+",");
  str.append("\"dir\":"        +std::to_string(msg->direction)+",");
  str.append("\"need_bt_off\":"+std::to_string(msg->needBtOff)+",");  
  
  str.append("\"dev_id\":"     +std::to_string(msg->devId)+",");
  str.append("\"group_id\":"   +std::to_string(msg->groupId)+",");
  str.append("\"dev_type\":"   +std::to_string(msg->devType)+",");
  str.append("\"dev_range\":"  +std::to_string(msg->devRange)+",");
  str.append("\"ad_esp\":"     +std::to_string(msg->addrEsp32)+",");
  str.append("\"ad_rm1\":"     +std::to_string(msg->addrRm1)+",");
  str.append("\"ad_rm2\":"     +std::to_string(msg->addrRm2)+",");
  str.append("\"mc1\":"        +std::to_string(msg->modCode1)+",");
  str.append("\"mc2\":"        +std::to_string(msg->modCode2)+",");
  str.append("\"msk1\":"       +std::to_string(msg->mask1)+",");
  str.append("\"msk2\":"       +std::to_string(msg->mask2)+",");
  str.append("\"pwr1\":"       +std::to_string(msg->pwr1)+",");
  str.append("\"pwr2\":"       +std::to_string(msg->pwr2)+",");
  str.append("\"txt_len\":"    +std::to_string(msg->txtLen)+",");
  str.append("\"txt\": \""     +std::string(msg->txt)+"\"");
  str.append("}");
  memccpy(data, str.c_str(), 0, 1024);
  return str.length();
}

int AN_serialConv::serializeJmmrData(_JMMR_STATE *jmmr, char *data){
  std::string str = "{";
  
  str.append("\"dev_id\":"   +std::to_string(jmmr->devId)+",");
  str.append("\"group_id\":" +std::to_string(jmmr->groupId)+",");
  str.append("\"dev_type\":" +std::to_string(jmmr->devType)+",");
  str.append("\"dev_range\":"+std::to_string(jmmr->devRange)+",");
  str.append("\"ad_esp\":"   +std::to_string(jmmr->esp32Addr)+",");
  str.append("\"ad_rm1\":"   +std::to_string(jmmr->rebMod[0].address)+",");
  str.append("\"ad_rm2\":"   +std::to_string(jmmr->rebMod[1].address)+",");
  str.append("\"mc1\":"      +std::to_string(jmmr->rebMod[0].mc)+",");
  str.append("\"mc2\":"      +std::to_string(jmmr->rebMod[1].mc)+",");
  str.append("\"msk1\":"     +std::to_string(jmmr->rebMod[0].mask)+",");
  str.append("\"msk2\":"     +std::to_string(jmmr->rebMod[1].mask)+",");
  str.append("\"pwr1\":"     +std::to_string(jmmr->rebMod[0].pwr)+",");
  str.append("\"pwr2\":"     +std::to_string(jmmr->rebMod[1].pwr)+",");
  str.append("\"batt_stt\":" +std::to_string(jmmr->devBattStt)+",");
  str.append("\"temper\":"   +std::to_string(jmmr->devTemper)+",");  
  str.append("\"txt_len\":"  +std::to_string(jmmr->infoLen)+",");
  str.append("\"txt\": \""   + std::string(jmmr->info)+"\"");
  str.append("}");
  memccpy(data, str.c_str(), 0, 1024);
  return str.length();
}

int AN_serialConv::serializeJmmrList(std::vector<_JMMR_STATE> jmmrsList, char *data)
{
  std::string str = "start___{\"cmd\":"+std::to_string(CMD_GET_JMMR_LIST)+",\"sender\":"+std::to_string(G_lJmrStt.esp32Addr)+",\"jmmr_list\":[";
  char tmpBuff[TXT_BUFF_LEN] = {'\0'};
  
  for(int i=0; i<jmmrsList.size(); i++){
    serializeJmmrData(&jmmrsList[i], tmpBuff);
    str.append(std::string(tmpBuff));
    if(i < jmmrsList.size()-1)str.append(",");
  }
  str.append("]}_stop\0\0\0\0");
  memccpy(data, str.c_str(), 0, str.length());
  return str.length();
}
