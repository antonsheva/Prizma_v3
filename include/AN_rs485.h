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


class AN_rs485: public AN_shiftDataArr{
private:
 
   
    void processMsg(_MSG_PACK *msg);
 
    void sendBtData(char *data);
    void sendJammListToBt();
    void sendBtResponse(BYTE cmd, uint32_t resp);
    void sendBtJmmrData(_MSG_PACK *msg);

    int transmitdataToBt(_SERIAL_PACK *sPack);

    void selectUart(int uart);

  public:
 
 
    void sendMsgToBt(_MSG_PACK *msg); 
    void prepMsg(_MSG_PACK *msg, BYTE iterNum);



    void init();
 
};



#endif