#include "AN_btCb.h"

AN_btCb::AN_btCb(/* args */){}

AN_btCb::~AN_btCb(){}

void AN_btCb::callback(esp_spp_cb_event_t event, esp_spp_cb_param_t * param){
    _SERIAL_PACK sPack;
   
    Serial.println("ESP_SPP_EVT ->  "+String(event));
    if (event == ESP_SPP_START_EVT) {
        Serial.println("ESP_SPP_OPEN_EVT");
    }
    
    if (event == ESP_SPP_SRV_OPEN_EVT) {
        sPack.cmd = EVENT_BT_CONNECT;
        Serial.println("ESP_SPP_SRV_OPEN_EVT Connected");
        xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY); 
        G_btConnect = true;
    }    
    
    if (event == ESP_SPP_CLOSE_EVT) {
        sPack.cmd = EVENT_BT_DISCONNECT;
        Serial.println("ESP_SPP_CLOSE_EVT disconnected");
        xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY); 
        G_btConnect = false; 
    }

    if(event == ESP_SPP_DATA_IND_EVT){ 
        Serial.println("ESP_SPP_DATA_IND_EVT  new data");
        _SERIAL_PACK sPack;
        sPack.cmd = CMD_BT_RECEIVE;
        xQueueSend(QueueBt, &sPack, portMAX_DELAY); 
    }
}




