#ifndef _AN_TASK_CMD_
#define _AN_TASK_CMD_

#include "Arduino.h"
#include "AN_objects.h"
#include "AN_macro.h"
#include "AN_shiftDataArr.h"
#include "AN_bt.h"

class AN_taskCmd {
private:

    
  static  int  AT();
  static  int  setATC(_MSG_PACK *msg);
  static  int  getATBT();           
  static  int  getATC();
  static  int  getJammList();
  static  int  processingResponseData(_MSG_PACK *msg);
  static  int  getJmmrData(_MSG_PACK *msg);    
  static  void applyPwr();
  static  void getDevParam();
  static  void init();
  static  void processingCmd(_MSG_PACK *msg);
  
 
  
  static void printJmmrList();
  static void printJmmrData(_MSG_PACK *msg);
  static void rmGetState();
  static void setJmmrData(_MSG_PACK *msg);
 
  static  void test();
  static  void btInit();
  static  void btStop();
  static  void getAllStack();
  static  void getStackSize(_MSG_PACK *msg);
 
  static  void setJmmrList();
  static  void updateLocalData(_MSG_PACK *msg);
  static  void addJmmr(_MSG_PACK *msg);
  static  void addJmmr(_JMMR_STATE *jmmr); 
  static  void selectRmModule(int sel, bool firstInit = 0);
  static  void sendCmdToRm(int cmd, int sel);  
public:
  AN_taskCmd(/* args */);
  ~AN_taskCmd();

  static void run(void *param);


};







#endif