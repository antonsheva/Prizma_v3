#ifndef _AN_BT_
#define _AN_BT_

#include "Arduino.h"
#include <BluetoothSerial.h> 
#include "AN_objects.h"
#include "AN_serial.h"


class AN_taskBt{
private:
  
public:
  AN_taskBt(/* args */);
  ~AN_taskBt();

  static void start();
  static void stop();
  static void send(_SERIAL_PACK sPack);
  static void receive();
  static void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
  static void run(void *data);
};



#endif
