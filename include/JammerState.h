#ifndef _REMOMOD_STATE_
#define _REMOMOD_STATE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define_types.h"
#include "define.h"
#include "esp_spp_api.h"


class JammerState{
private:
 

    typedef struct{
        BYTE   mc;
        DWORD  mask;
        BYTE   address;
        BYTE   echo;
        BYTE   pwr;
        float  vcpu;
        float  temp;
        char   info[TXT_BUFF_LEN-128] = {0};
        size_t  infoLen = 0;
    }struct_rebMod;

    typedef struct{
        uint32_t      status    = 0;         /*!< status */
        uint32_t      sppClient = 0;         /*!< The connection handle */
        esp_bd_addr_t rem_bda;       
    }_bt_param;
    

public:


  
    JammerState(/* args */);
    ~JammerState();
    DDWORD devId;      
    BYTE   groupId;
    BYTE   devType;   // 1 - A; 2 - B
    BYTE   devRange;
    BYTE   esp32Addr;
    char   info[TXT_BUFF_LEN-128] = {0};
    BYTE   infoLen = 0;
    _bt_param bt;
    struct_rebMod rebMod[2];
};











#endif
