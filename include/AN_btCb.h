#ifndef _AN_BT_CB_
#define _AN_BT_CB_
#include "Arduino.h"
#include "AN_objects.h"


class AN_btCb{
private:
  /* data */
public:
  AN_btCb(/* args */);
  ~AN_btCb();
  static void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
};





#endif
