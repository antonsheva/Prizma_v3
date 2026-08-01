#ifndef _AN_PREFERENCE_
#define _AN_PREFERENCE_

#include "main.h"
#include "nvs.h"
#include "nvs_flash.h"
class AN_pref
{
private:
 

    void setParam(char* param, BYTE val);
    void setParam(char* param, uint64_t val);

public:
 	AN_pref(){};
    ~AN_pref(){}

    void init();

    void setGroupId(BYTE groupId);
    void setDevId(uint64_t id);
    void setDevType(BYTE type);
    void setDevRange(BYTE range);
    void setAddrEsp(BYTE addr);
    void setAddrRm(BYTE addrRm1, BYTE addrRm2);
    void setPwr(BYTE pwr1, BYTE pwr2);

    void printAddresses();
    void getDevParam();
};

#endif


