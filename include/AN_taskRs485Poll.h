#ifndef _AN_TASK_RS485_POLL_
#define _AN_TASK_RS485_POLL_


#include "main.h"
#include "AN_rs485.h"

class AN_taskRs485Poll{
private:

  static int checkStopPoll(_MSG_PACK *msg);

  static bool checkExistAddr(BYTE addr);

public:
  AN_taskRs485Poll(/* args */);
  ~AN_taskRs485Poll();



  static void run(void *param);
};







#endif