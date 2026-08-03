#include "../include/AN_bt.h"
#include "AN_bt.h"


 
 

// GPIO пин светодиода (согласно схеме - GPIO 1)
#define LED_GPIO_PIN GPIO_NUM_1

// Теги для логирования
static const char *TAG = "BLED_LED_Control";

 

// Буфер для приема данных

#define SPP_RUNNING      0x01
#define SPP_CONNECTED    0x02
#define SPP_CONGESTED    0x04
#define SPP_DISCONNECTED 0x08
#define SPP_CLOSED       0x10

#define RX_QUEUE_SIZE 512
#define TX_QUEUE_SIZE 32

#define SPP_CONGESTED_TIMEOUT 1000





static std::string devName;

 

  

bool AN_bt::chaeckEvents(){
    return((xEventGroupWaitBits(EventGroupSpp, SPP_CONGESTED, pdFALSE, pdTRUE, SPP_CONGESTED_TIMEOUT) & SPP_CONGESTED) != 0);
}

void AN_bt::saveConnectionData(esp_spp_cb_param_t *param){
    memccpy(G_lJmrStt.bt.rem_bda, param->open.rem_bda, 0, ESP_BD_ADDR_LEN);
    G_lJmrStt.bt.sppClient = param->open.handle;
    G_lJmrStt.bt.status = SPP_CONNECTED;
    G_btConnect = true;
}

void AN_bt::clearConnectionData(){
    memset(G_lJmrStt.bt.rem_bda, 0, ESP_BD_ADDR_LEN);
    G_lJmrStt.bt.sppClient = 0;
    G_lJmrStt.bt.status = SPP_DISCONNECTED;
} 

 
 
void AN_bt::spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
    _SERIAL_PACK sPack;

    AN_print("ESP_SPP_EVT ->  "+std::to_string(event));

    switch (event) {
      case ESP_SPP_INIT_EVT:
          AN_print("ESP_SPP_INIT_EVT");
          esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, "SPP_LED_Server");
          xEventGroupSetBits(EventGroupSpp, SPP_RUNNING);
      break;

      case ESP_SPP_DISCOVERY_COMP_EVT:
              ESP_LOGI(TAG, "Search of devices is finish");
      break;

      case ESP_SPP_OPEN_EVT:
        ESP_LOGI(TAG, "Bluetooth connect  с %s", param->open.rem_bda);

        xEventGroupClearBits(EventGroupSpp, SPP_DISCONNECTED);
        xEventGroupSetBits(EventGroupSpp, SPP_CONNECTED);
        xEventGroupSetBits(EventGroupSpp, SPP_CONGESTED);
      break;

      case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(TAG, "Bluetooth connectin close!!!---");

        memset(G_lJmrStt.bt.rem_bda, 0, ESP_BD_ADDR_LEN);
        G_lJmrStt.bt.sppClient = 0;
        G_lJmrStt.bt.status = SPP_DISCONNECTED;
        G_btConnect = 0;
        // sPack.cmd = EVENT_BT_DISCONNECT;                
        // xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);
        
        xEventGroupSetBits(EventGroupSpp,    SPP_DISCONNECTED);
        xEventGroupSetBits(EventGroupSpp,    SPP_CONGESTED);
        xEventGroupSetBits(EventGroupSpp,    SPP_CLOSED);
        xEventGroupClearBits(EventGroupSpp,  SPP_CONNECTED);

      break;

    case ESP_SPP_CONG_EVT://connection congestion status changed
      if(param->cong.cong){
          xEventGroupClearBits(EventGroupSpp, SPP_CONGESTED);
      } else {
          xEventGroupSetBits(EventGroupSpp, SPP_CONGESTED);
      }
      AN_print("ESP_SPP_CONG_EVT");
    break;
    
    case ESP_SPP_START_EVT:
            ESP_LOGI(TAG, "SPP is runnunig. Connection is expected ...");
    break;

    case ESP_SPP_DATA_IND_EVT:
            // AN_print(std::string((char*)param->data_ind.data).c_str());
            // sPack.data = static_cast<char *>(malloc(param->data_ind.len));
            // if (sPack.data == NULL) {
            //     ESP_LOGE("UART", "Malloc failed");
            //     break;
            // }
            // sPack.len = param->data_ind.len;
            // memccpy(sPack.data, param->data_ind.data, 0, param->data_ind.len);
            // xQueueSend(QueueBtReceive, &sPack, portMAX_DELAY);

    break;

    case ESP_SPP_WRITE_EVT:
          if (param->write.status == ESP_SPP_SUCCESS) {
              if(param->write.cong){
                xEventGroupClearBits(EventGroupSpp, SPP_CONGESTED);
              }
              AN_print("ESP_SPP_WRITE_EVT");
          } else {
              AN_print("ESP_SPP_WRITE_EVT failed!");
          }
          xSemaphoreGive(SemaphoreTxDone);//we can try to send another packet
    break;

    case ESP_SPP_SRV_OPEN_EVT://Server connection open
            if (param->srv_open.status == ESP_SPP_SUCCESS) {  
              if (!G_btConnect && !G_lJmrStt.bt.sppClient){
                AN_print(" --- CONNECCTTT  _____----");
                // G_btConnect = true;
                // sPack.cmd = EVENT_BT_CONNECT;                
                // xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);
                saveConnectionData(param);
                xEventGroupClearBits(EventGroupSpp, SPP_DISCONNECTED);
                xEventGroupSetBits(EventGroupSpp, SPP_CONNECTED);
              }
            }else{
                AN_print("Connect error");
            } 
    break;

            

    default:
        ESP_LOGD(TAG, "SPP event: %d", event);
        break;
  }
    processEvent(event, param);
}
 
void AN_bt::processEvent(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
    _SERIAL_PACK sPack;
    if(event == 27){
      Serial.println("BT disconnect --- ");
      G_lJmrStt.bt.sppClient = 0;
      G_lJmrStt.bt.status = SPP_DISCONNECTED;
      G_btConnect = false;
      sPack.cmd = EVENT_BT_DISCONNECT;                
      xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);
    }
    if(event == 34){
      Serial.println("BT connect --- ");
      G_btConnect = true;
      sPack.cmd = EVENT_BT_CONNECT;                
      xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);
    }
    if(event == 30){
      Serial.println("BT data --- ");      
      AN_print(std::string((char*)param->data_ind.data).c_str());
        sPack.data = static_cast<char *>(malloc(param->data_ind.len));
        if (sPack.data == NULL) {
            ESP_LOGE("UART", "Malloc failed");
        }
        sPack.len = param->data_ind.len;
        memccpy(sPack.data, param->data_ind.data, 0, param->data_ind.len);
        xQueueSend(QueueBtReceive, &sPack, portMAX_DELAY);
    }

}

void AN_bt::gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
      AN_print("ESP_BT_GAP_EVT ->  "+std::to_string(event));
    switch (event) {
        case ESP_BT_GAP_AUTH_CMPL_EVT:
            if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
                ESP_LOGI(TAG, "Auth OK: %s", param->auth_cmpl.device_name);
            } else {
                ESP_LOGE(TAG, "Auth error: %d", param->auth_cmpl.stat);
            }
            break;

        default:
            ESP_LOGD(TAG, "GAP event: %d", event);
            break;
    }
}

 

void AN_bt::init(void){

 
    devType = G_lJmrStt.devType == DEV_TYPE_A ? "A" : "B";
    devAddr = std::to_string(G_lJmrStt.esp32Addr);
    devName = "Prizma_JMR_"+devType+"_"+devAddr;
 
    AN_print("Bluetooth init...");




    if(EventGroupBt){
        xEventGroupClearBits(EventGroupBt, 0xFFFFFF);
    }

    if(SemaphoreTxDone != NULL){
        xSemaphoreTake(SemaphoreTxDone, 0);
    }  

    if(EventGroupSpp){
        xEventGroupClearBits(EventGroupSpp, 0xFFFFFF);
        xEventGroupSetBits(EventGroupSpp, SPP_CONGESTED);
        xEventGroupSetBits(EventGroupSpp, SPP_DISCONNECTED);
        xEventGroupSetBits(EventGroupSpp, SPP_CLOSED);
    }
    esp_bt_controller_mem_release(ESP_BT_MODE_BLE);   

 
    esp_bt_controller_config_t cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if(esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_IDLE){
        esp_bt_controller_init(&cfg);
        while(esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_IDLE){}
    }
    if(esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_INITED){
        if (esp_bt_controller_enable(ESP_BT_MODE_BTDM)) {
            AN_print("BT Enable failed");
            return;
        }
    }
    if(esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED){
        AN_print("BT Enable");
    }

    
    // Инициализация Bluedroid стека
    esp_err_t  ret = esp_bluedroid_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid: %s", esp_err_to_name(ret));
        return;
    }
    
    ret = esp_bluedroid_enable();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid enable error: %s", esp_err_to_name(ret));
        return;
    }
 
    esp_bt_gap_set_device_name(devName.c_str());
    
    // Регистрация callback функций
    esp_bt_gap_register_callback(gap_callback);
    esp_spp_register_callback(spp_callback);
    
    // Инициализация SPP
    esp_spp_cfg_t _cnfg_1;
    _cnfg_1.mode = ESP_SPP_MODE_CB;
    _cnfg_1.enable_l2cap_ertm = false;
    _cnfg_1.tx_buffer_size = 4096;

    esp_spp_enhanced_init(&_cnfg_1);
    
    // Настройка discoverability и connectability
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    ESP_LOGI(TAG, "Bluetooth was init. Device name: %s", devName.c_str());
    G_btStart = true;
}

bool AN_bt::_stop_bt()
{
  AN_print("_stop_bt");
    if (G_btStart){
        if(G_lJmrStt.bt.sppClient)
            esp_spp_disconnect(G_lJmrStt.bt.sppClient);
        esp_spp_deinit();
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
       
    }
    G_lJmrStt.bt.sppClient = 0;
    G_btStart = false;
    return true;
}

void AN_bt::end(){
  _stop_bt();
}
void AN_bt::sendJmmrList(){
  AN_serialConv sc;
  char data[MAX_SERIAL_DATA_LEN];
  // int len = sc.serializeJmmrList(G_jmmrsList, data);
  AN_print(data);
  // SerialBT.write((const BYTE*)data, len);
}


 
AN_bt::AN_bt(/* args */){}
AN_bt::~AN_bt(){
  // _stop_bt();
}

