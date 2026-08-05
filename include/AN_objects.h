#ifndef _AN_OBJECTS_
#define _AN_OBJECTS_

#include <vector> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h" 
#include "driver/uart.h"
  
#include <BluetoothSerial.h>
 
#include "define.h"
#include "define_types.h"
#include <string>

 
typedef struct{
    BYTE   mc;
    DWORD  mask;
    BYTE   address;
    BYTE   echo;
    BYTE   pwr;
    float  vcpu;
    float  temp;
    char   info[TXT_INFO_LEN] = {0};
    size_t  infoLen = 0;
}struct_rebMod;

typedef struct{
    uint32_t      status    = 0;         /*!< status */
    uint32_t      sppClient = 0;         /*!< The connection handle */
    esp_bd_addr_t rem_bda;       
}_bt_param;

typedef struct{ 

    DDWORD devId;      
    BYTE   groupId;
    BYTE   devType;   // 1 - A; 2 - B
    BYTE   devRange;
    BYTE   esp32Addr;
    char   info[TXT_INFO_LEN] = {0};
    BYTE   infoLen = 0;
    WORD   devTemper   = 0;
    WORD   devBattStt  = 0;
    struct_rebMod rebMod[2];
    _bt_param bt;
}_JMMR_STATE;



typedef struct{
    BYTE dataLen = 0;
    BYTE data[1024];
    WORD marker = 0; 
    int  packQty;
    int  lastPackLen;
}_RS485_data;
 
 
typedef struct{
    // BYTE    rmNum       = 0;
    int    cmd         = 0;
    int    cmdType     = 0;
    int    direction   = 0;
    int    sender      = 0;
    int    response    = 0; 
    
    int    rmNum       = 0;

    int    addrEsp32   = 0;
    int    modCode1    = 0;
    int    modCode2    = 0;   
    int    mask1       = 0;
    int    mask2       = 0;
    int    addrRm1     = 0;
    int    addrRm2     = 0;
    int    pwr1        = 0;
    int    pwr2        = 0;  
    
    DDWORD devId       = 0;
    int    groupId     = 0;    
    int    devType     = 0;
    int    devRange    = 0;  
    WORD   devTemper   = 0;
    WORD   devBattStt  = 0;
    
    int    jmmrListLen    = 0;
    int    subscribersQty = 0;

    bool   updtLocalJmmr = 1;

    int    txtLen   = 0;
    char   txt[TXT_BUFF_LEN-128] = {0};

}_MSG_PACK;

typedef struct{
    BYTE esp32;
    BYTE rm1;
    BYTE rm2;
}_ADDRESSESS;

typedef struct{
    BYTE opCodeList [16]  = {0};
    BYTE opCodeQty        = 0;
    BYTE rmSel            = 0;
    BYTE cmd              = 0;
    bool swtchActDev      = 0;
}_RM_AUT;

typedef struct{
    char *data;
    BYTE len = 0;
}_BT_PACK;


typedef struct{
    BYTE  cmd         = 0;
    BYTE  cmdType     = 0;
    BYTE  code        = 0;
}_MSG_INTERNAL;
 
typedef struct {
    int  len=0;
    char *data;
    BYTE dir=0;
    BYTE cmd=0;
} _SERIAL_PACK;

typedef struct {
    int     len;
    char  data[];
} _S_PACK;

typedef struct {
    int  len;
    char data[32];
} _RM_PACK;
  

extern std::vector<_JMMR_STATE> G_jmmrsList;
 
extern _MSG_PACK G_rm_msg;
extern _MSG_PACK G_485_msg;
 
 
extern _JMMR_STATE G_lJmrStt;

extern QueueHandle_t QueueBt;

extern QueueHandle_t QueueBtSend        ;
extern QueueHandle_t QueueBtReceive     ;

extern QueueHandle_t QueueRs485Send     ;
extern QueueHandle_t QueueRs485Receive  ;

extern QueueHandle_t QueuePrefs   ;
extern QueueHandle_t QueuePwrAut        ;

extern QueueHandle_t QueueLeds          ;
extern QueueHandle_t QueueCmd           ;
extern QueueHandle_t QueueRs485Pool     ;

extern QueueHandle_t QueueUsbEvent      ;
extern QueueHandle_t QueueUsb           ;

extern QueueHandle_t QueueRs485Event    ; 
extern QueueHandle_t QueueRs485         ;
extern QueueHandle_t QueueRmEvent       ; 

extern QueueHandle_t QueueBtSend        ;
extern QueueHandle_t QueueBtReceive     ; 


  
extern SemaphoreHandle_t SemaphoreCbBt     ;
extern SemaphoreHandle_t SemaphoreCbRm     ; 

extern SemaphoreHandle_t  SemaphoreTxDone ;
extern EventGroupHandle_t EventGroupSpp   ;
extern EventGroupHandle_t EventGroupBt    ;  
 
extern BYTE G_subscribersQty;

extern BYTE G_voltToLeds;  
extern BYTE G_ledsStste[2];

extern char G_txtJsonBuff[3968];
extern char G_txtSerialBuff[4096];

extern bool G_updatePref; 
extern bool G_led_ccl_5;
extern bool G_btConnect;
extern bool G_btStart;

extern BYTE G_pwrMode;
extern BYTE G_waitResponse;
extern BYTE G_serialBusy;
 
extern int G_serialWaitTm    ;
extern int G_serialEndOfPack ;
extern int G_serialDataSrc   ;
 
extern DWORD G_wait485PackCnt;
extern DWORD G_waitBtPackCnt;
extern DWORD G_waitBtConnect;

extern DWORD G_pauseRmDataCnt;  
extern int G_rebModAut_tm; 


extern int G_msgTxtDataLen;
extern int G_pauseBtDataCnt;

extern char G_msgTxtData[512];
 
extern bool G_eventExpected [32];
extern bool G_eventOccurred[32];


#define UART_SELECT_RS485  0
#define UART_SELECT_RM1    1
#define UART_SELECT_RM2    2
extern int  G_uartSelector;
extern bool G_mtxUart2Busy;

void AN_print(std::string str, int len = 0);

void initObjects();

#endif

