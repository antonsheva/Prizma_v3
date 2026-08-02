#include "AN_taskBtSend.h"
AN_taskBtSend::AN_taskBtSend(){}
AN_taskBtSend::~AN_taskBtSend(){}
 

#define SPP_RUNNING      0x01
#define SPP_CONNECTED    0x02
#define SPP_CONGESTED    0x04
#define SPP_DISCONNECTED 0x08
#define SPP_CLOSED       0x10

#define RX_QUEUE_SIZE 512
#define TX_QUEUE_SIZE 32

#define SPP_CONGESTED_TIMEOUT 1000
 

 
const  uint16_t SPP_TX_MAX = 330;
static uint16_t _spp_tx_buffer_len;
static uint8_t  _spp_tx_buffer[SPP_TX_MAX];
 
bool AN_taskBtSend::stop(){

    return 0;
}

void AN_taskBtSend::init(){
 
}

bool AN_taskBtSend::_spp_send_buffer(){
    AN_bt bt;
    AN_print("_spp_send_buffer--");
    if((xEventGroupWaitBits(EventGroupSpp, SPP_CONGESTED, pdFALSE, pdTRUE, SPP_CONGESTED_TIMEOUT) & SPP_CONGESTED) != 0){
        if(!G_lJmrStt.bt.sppClient){
            AN_print("SPP Client Gone!");
            return false;
        }
        AN_print("SPP Write "+std::to_string(_spp_tx_buffer_len));
        AN_print("data ->  "+std::string((char*)_spp_tx_buffer));        
        esp_err_t err = esp_spp_write(G_lJmrStt.bt.sppClient, _spp_tx_buffer_len, _spp_tx_buffer);
        if(err != ESP_OK){
            AN_print("SPP Write Failed!");
            return false;
        }
        _spp_tx_buffer_len = 0;
        if(xSemaphoreTake(SemaphoreTxDone, 1000/portTICK_PERIOD_MS) != pdTRUE){
            AN_print("SPP Ack Failed!");
            return false;
        }
        return true;
    }
    AN_print("SPP Write Congested!");
    return false;
}

void AN_taskBtSend::run(void *param){
  const uint16_t SPP_TX_MAX = 330;
  AN_bt bt;
  _SERIAL_PACK packet;
  size_t len = 0, to_send = 0;
  char * data = NULL;
  init();

  for (;;) {
    if(QueueBtSend && xQueueReceive(QueueBtSend, &packet, portMAX_DELAY) == pdTRUE){
        if(packet.len <= (SPP_TX_MAX - _spp_tx_buffer_len)){
            AN_print("QueueBtSend ->"+std::string(packet.data));
            memcpy(_spp_tx_buffer+_spp_tx_buffer_len, packet.data, packet.len);
            _spp_tx_buffer_len+=packet.len;
            AN_print("tp - 4");
            free(packet.data);
            AN_print("tp - 5");

            if(SPP_TX_MAX == _spp_tx_buffer_len || uxQueueMessagesWaiting(QueueBtSend) == 0){
                _spp_send_buffer();
            }
        } else {
            len = packet.len;
            data = packet.data;
            to_send = SPP_TX_MAX - _spp_tx_buffer_len;
            memcpy(_spp_tx_buffer + _spp_tx_buffer_len, data, to_send);
            _spp_tx_buffer_len = SPP_TX_MAX;
            data += to_send;
            len -= to_send;
            if(!_spp_send_buffer()){
                len = 0;
            }
            while(len >= SPP_TX_MAX){
                memcpy(_spp_tx_buffer, data, SPP_TX_MAX);
                _spp_tx_buffer_len = SPP_TX_MAX;
                data += SPP_TX_MAX;
                len -= SPP_TX_MAX;
                if(!_spp_send_buffer()){
                    len = 0;
                    break;
                }
            }
            if(len){
                memcpy(_spp_tx_buffer, data, len);
                _spp_tx_buffer_len+=len;
                if(uxQueueMessagesWaiting(QueueBtSend) == 0){
                    _spp_send_buffer();
                }
            }
            free(packet.data);
        }
    } else {
        AN_print("Something went horribly wrong");
    }
}
vTaskDelete(NULL);
 
}

#undef _TX_BUFF    
#undef _TX_BUFF_LEN