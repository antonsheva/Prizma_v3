#ifndef _AN_TASK_RM_RECEIVE_
#define _AN_TASK_RM_RECEIVE_

#include "main.h"

class AN_taskRmReceive{
private:
  /* data */
public:
  AN_taskRmReceive(/* args */);
  ~AN_taskRmReceive();

  static void getDevInfo(String data);
 
  static void initTimer();
  static void fillDevParams(int dataArrLen, String *data);
  static void callback();

  static void run(void *param);
};


#endif

