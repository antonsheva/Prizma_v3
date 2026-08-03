#ifndef _AN_BT_
#define _AN_BT_

#include "main.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "freertos/queue.h"

#include "AN_objects.h"
 

class AN_bt{
private:
  


static void saveConnectionData(esp_spp_cb_param_t *param);



  
  
  public:
  AN_bt(/* args */);
  ~AN_bt();
  




bool chaeckEvents();

static void clearConnectionData();

 

  // size_t write(char *buffer, size_t size);


  // static bool _spp_send_buffer();
  static void spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
  static void processEvent(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
  static void gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
  static bool _stop_bt();
  // static esp_err_t _spp_queue_packet(uint8_t *data, size_t len);
   
  std::string devType;
  std::string devAddr;


  void init();

  void end();

  void sendJmmrList();
};



#endif
