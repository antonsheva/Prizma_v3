#include "../include/AN_taskRs485Send.h"
 
void AN_taskRs485Send::run(void *param){
  _MSG_PACK msg;
  AN_serialConv serialConv;
  FastCRC16 crc;
 
  int len         ;
  int packQty     ;
  int lastPackLen ;
  WORD crc16;
  for(;;){
    xQueueReceive(QueueRs485Send, &msg, portMAX_DELAY);
    len    = serialConv.serializeRs485Data(&msg, G_txtJsonBuff);
    crc16  = crc.modbus((const uint8_t*) G_txtJsonBuff, len);

    len = sprintf(G_txtSerialBuff, "start___%s_%X_stop", G_txtJsonBuff, crc16);
    
    packQty     = len/120+1;
    lastPackLen = len%120;
    AN_print(G_txtSerialBuff);
    for(int i=0; i < packQty; i++){
        len = (i < packQty-1) ? 120 : lastPackLen;
        gpio_set_level(PIN_RS485_DIR_DRV,1);
        uart_write_bytes(UART_RS485, &G_txtSerialBuff[i*120], len);//(&G_txtSerialBuff[i*120], len);     
        uart_wait_tx_done(UART_RS485, 1000); 
        gpio_set_level(PIN_RS485_DIR_DRV,0);  
        vTaskDelay(50/portTICK_PERIOD_MS);
    }  
  }
}

AN_taskRs485Send::AN_taskRs485Send(/* args */)
{
  
}

AN_taskRs485Send::~AN_taskRs485Send()
{
}














