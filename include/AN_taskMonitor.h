#ifndef _AN_MONITOR_
#define _AN_MONITOR_

#include "main.h"

class AN_taskMonitor{
private:
  /* data */
public:
  AN_taskMonitor(/* args */);
  ~AN_taskMonitor();

  static void run(void *param);
};






#endif


