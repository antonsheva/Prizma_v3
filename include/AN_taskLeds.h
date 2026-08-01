#ifndef _AN_TASK_LEDS_
#define _AN_TASK_LEDS_
 
#include "Arduino.h"
#include "define_types.h" 
#include "define.h"
#include "AN_objects.h"


class AN_taskLeds{
private:
  /* data */
public:
  AN_taskLeds(/* args */);
  ~AN_taskLeds();

  static void setState(BYTE stt);

  static void run(void *param);
};




#endif


