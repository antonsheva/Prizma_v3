#ifndef _AN_RS485_
#define _AN_RS485_
#include "define_types.h"
#include "AN_objects.h"
#include "AN_serialConv.h"
#include "AN_serial.h"

#define BROADCAST_ADDR 0xFF
#define DEV_STATUS_MASTER 1
#define DEV_STATUS_SLAVE  0


#define RS485_TMP_BUFF_SIZE 512


class AN_commRs485Bt: public AN_shiftDataArr{
private:
  
  public:
    void sendBtJammList();
    void sendBtResponse(BYTE cmd, uint32_t resp);  
    void sendBtJmmrData(_MSG_PACK *msg); 
    void sendBtJmmrData(_JMMR_STATE *jmmr);
    void sendMsgToBt(_MSG_PACK *msg); 
    void prepMsg(_MSG_PACK *msg, BYTE iterNum);
 
 
};



#endif