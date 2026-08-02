#ifndef _AN_TASK_BT_SEND_
#define _AN_TASK_BT_SEND_

#include "main.h"
 
  


class AN_taskBtSend{
private:
  /* data */
public:
  AN_taskBtSend(/* args */);
  ~AN_taskBtSend();
  static esp_err_t _spp_queue_packet(uint8_t *data, size_t len);
  static bool stop();
  static void init();
  static bool _spp_send_buffer();
  static void run(void *param);
};








#endif


