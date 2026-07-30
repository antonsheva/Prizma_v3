#ifndef _AN_SHIFT_
#define _AN_SHIFT_


#include "AN_objects.h"
#include "esp_log.h"

class AN_shiftDataArr{
private:
    /* data */
public:
    AN_shiftDataArr(/* args */);
    ~AN_shiftDataArr();
    void loadMsgToJmrStt(_MSG_PACK *msg, JammerState *jmmr);
    void loadJmmrStateToMsg(_MSG_PACK *msg, JammerState *jmmr);
    void copyJmmr(JammerState *jmmr1, JammerState *jmmr2);
    void printJmmrData(JammerState *jmmr);
    void printMsg(_MSG_PACK *msg);
    void printJmmrList();

};

#endif