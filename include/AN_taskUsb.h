#ifndef _AN_TASK_USB_
#define _AN_TASK_USB_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "portmacro.h"
#include "AN_objects.h"
#include "define.h" 
#include "AN_serial.h" 

class AN_taskUsb{
private:



public:
  AN_taskUsb(/* args */);
  ~AN_taskUsb();

  static void callback();

  static void run(void *param);
};







#endif