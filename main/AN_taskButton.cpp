#include "../include/AN_taskButton.h"



AN_taskButton::AN_taskButton(/* args */)
{
}

AN_taskButton::~AN_taskButton()
{
}
 
void AN_taskButton::run(void *param){
    _SERIAL_PACK sPack;
    bool    bttnSwch        = 0;
    int     bttnPressCnt    = 0;
    int     bttnUnpressCnt  = 0;
    int     bttn            = 0;
    for(;;){
        bttn = gpio_get_level(PIN_PWR_BUTTON);
        if(!bttnSwch){
            if(bttn)bttnUnpressCnt++;
            else    bttnUnpressCnt = 0;
            if(bttnUnpressCnt>100){
                bttnSwch = 1;             
            }
        }else{
            if(!bttn)bttnPressCnt++;
            else     bttnPressCnt = 0;
            if(bttnPressCnt > 50){
                bttnSwch = 0;
                sPack.cmd = EVENT_CODE_BTTN_ON;
                xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);   
                AN_print("stt 2");             
            }
        }  
        vTaskDelay(10/portTICK_PERIOD_MS);
    }  
}