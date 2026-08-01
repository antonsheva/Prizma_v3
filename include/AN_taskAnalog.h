#ifndef _AN_TASK_ANALOG_PARAM_
#define _AN_TASK_ANALOG_PARAM_

#include "main.h"
#include "sdkconfig.h"
// #include <esp_adc/adc_oneshot.h>

class AN_taskAnalog{
private:
  /* data */
public:
  AN_taskAnalog(/* args */);
  ~AN_taskAnalog();

  static void run(void *param);
};






#endif


