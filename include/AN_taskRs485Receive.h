#ifndef _AN_TASK_RS485_RECEIVE_
#define _AN_TASK_RS485_RECEIVE_

#include "main.h"

class AN_taskRs485Receive{
private:
  /* data */
public:
  AN_taskRs485Receive(/* args */);
  ~AN_taskRs485Receive();

  static void callback();
  static void run(void *param);


};




#endif



