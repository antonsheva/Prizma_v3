#ifndef _AN_TASK_PWR_AUT_
#define _AN_TASK_PWR_AUT_

#include "main.h"
#include "AN_macro.h"
class AN_taskPwrAut{
private:
  static void eventPwrOff();
  static void eventAplayChanges();
  static void eventDisconnect();
  static void eventConnect();
  static void eventBtOn();



public:
  AN_taskPwrAut(/* args */);
  ~AN_taskPwrAut();

  // BYTE ledsCode[4];  
  // bool btEnSwch;


  static void run(void *param);
};





#endif
