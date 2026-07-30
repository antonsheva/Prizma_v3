#include "../include/JammerState.h"
#define TXT_BUF_LEN 512

JammerState::JammerState(/* args */)
{
    for(int rm=0; rm<2; rm++){
        rebMod[rm].mc = 0;
        rebMod[rm].mask = 0;
        rebMod[rm].address = 0;
        rebMod[rm].echo = 0;
        rebMod[rm].vcpu = 0;
        rebMod[rm].temp = 0;
        rebMod[rm].infoLen = 0;
        memset(rebMod[rm].info, 0, TXT_BUF_LEN-128);
    }
    info[0] = '_';
    info[1] = '_';
    info[2] = '_';
    
}

JammerState::~JammerState()
{
}




