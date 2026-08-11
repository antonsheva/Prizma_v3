#include "../include/AN_objects.h"

_JMMR_STATE G_lJmrStt;
 

QueueHandle_t QueueBt         = NULL;

QueueHandle_t QueueBtSend     = NULL;
QueueHandle_t QueueBtReceive  = NULL; 
 
QueueHandle_t QueueRs485Send     = NULL;
QueueHandle_t QueueRs485Receive  = NULL;

QueueHandle_t QueuePrefs   = NULL;
QueueHandle_t QueueLeds          = NULL;
QueueHandle_t QueuePwrAut        = NULL; 
QueueHandle_t QueueCmd           = NULL;
QueueHandle_t QueueRs485Pool     = NULL;

QueueHandle_t QueueUsbEvent      = NULL; 
QueueHandle_t QueueUsb           = NULL; 

QueueHandle_t QueueRs485Event    = NULL; 
QueueHandle_t QueueRs485         = NULL;

QueueHandle_t QueueRmEvent       = NULL; 
 
 

  
  
SemaphoreHandle_t SemaphoreCbBt    = NULL;
SemaphoreHandle_t SemaphoreCbRm    = NULL;

SemaphoreHandle_t SemaphoreTxDone  = NULL;
EventGroupHandle_t EventGroupSpp   = NULL;
EventGroupHandle_t EventGroupBt    = NULL; 

std::vector<_JMMR_STATE> G_jmmrsList;

_RS485_data RS485_data;


_MSG_PACK G_rm_msg;
_MSG_PACK G_485_msg;
  
int G_pauseBtDataCnt = 0;

BYTE G_subscribersQty = 0;  

char G_txtJsonBuff[3968];
char G_txtSerialBuff[4096];
 

BYTE G_waitResponse = 0;
BYTE G_serialBusy = 0;

DWORD G_wait485PackCnt  = 0;
DWORD G_waitBtPackCnt  = 0;
DWORD G_waitBtConnect  = 0;

DWORD G_pauseRmDataCnt = 0;
int G_rebModAut_tm = 0; 


int G_serialWaitTm    = 0;
int G_serialEndOfPack = 0;
int G_serialDataSrc   = 0;
 

bool G_btConnect    = false;
bool G_btStart      = false;

BYTE G_pwrMode      = false;

BYTE G_voltToLeds   = 0;  
BYTE G_ledsState[2] = {0};

int G_msgTxtDataLen;

char G_msgTxtData[TXT_BUFF_LEN];
 
bool G_eventExpected[32]    = {false};
bool G_eventOccurred[32]    = {false};   

int  G_uartSelector = 0;
bool G_mtxUart2Busy = 0;
int  G_needBtOff = 0;

void AN_print(std::string str, int len){
    if(len){
      str.insert(len, "\n\r\0");
    }else{
      str.append("\n\r\0");
    }
    uart_write_bytes(UART_USB, str.c_str(), str.length());
    // uart_wait_tx_done(UART_USB, 100);
}

void initObjects()
{ 
 
    QueueRs485Send       = xQueueCreate(2, sizeof(_MSG_PACK));
    QueueRs485Receive    = xQueueCreate(2, 1024);
    QueuePrefs           = xQueueCreate(2,  sizeof(_MSG_PACK));
 
    QueuePwrAut          = xQueueCreate(16, sizeof(_SERIAL_PACK));
    QueueLeds            = xQueueCreate(4,  4);
      
    QueueCmd             = xQueueCreate(4, sizeof(_MSG_PACK)); 
    QueueRs485Pool       = xQueueCreate(4, sizeof(_MSG_PACK)); 

 

    QueueUsbEvent        = xQueueCreate(4, sizeof(uart_event_t));    
    QueueUsb             = xQueueCreate(4, sizeof(_SERIAL_PACK));    

    QueueRs485Event      = xQueueCreate(4, sizeof(uart_event_t));  
    QueueRs485           = xQueueCreate(16, sizeof(_SERIAL_PACK));

    QueueRmEvent         = xQueueCreate(32, sizeof(_RM_AUT));  
 
  
    
    QueueBt              = xQueueCreate(8, sizeof(_SERIAL_PACK));  
    QueueBtSend          = xQueueCreate(16, sizeof(_SERIAL_PACK));  
    QueueBtReceive       = xQueueCreate(16, sizeof(_SERIAL_PACK));   
 
  
    SemaphoreCbBt      = xSemaphoreCreateBinary();
    SemaphoreCbRm      = xSemaphoreCreateBinary();
    SemaphoreTxDone    = xSemaphoreCreateBinary(); 

    EventGroupBt       = xEventGroupCreate();
    EventGroupSpp      = xEventGroupCreate();    
}


