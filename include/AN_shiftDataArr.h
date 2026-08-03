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
    void loadMsgToJmrStt(_MSG_PACK *msg, _JMMR_STATE *jmmr, bool rmOnly = 0);
 
    void loadJmmrStateToMsg(_MSG_PACK *msg, _JMMR_STATE *jmmr);
    void copyJmmr(_JMMR_STATE *jmmr1, _JMMR_STATE *jmmr2, bool rmDataOnly = 0);
 
    void printJmmrData(_JMMR_STATE *jmmr);
    void printMsg(_MSG_PACK *msg);
    void printJmmrList();

};

#endif