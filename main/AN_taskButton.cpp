#include "../include/AN_taskButton.h"



AN_taskButton::AN_taskButton(/* args */)
{
}

AN_taskButton::~AN_taskButton()
{
}
 
#define BTTN_STATE_UNPRESS    0
#define BTTN_STATE_PRESS      1
#define BTTN_STATE_LONG_PRESS 2

void AN_taskButton::run(void *param){
    _SERIAL_PACK sPack;
    int  bttnPressCnt    = 0;
    int  bttnUnpressCnt  = 0;
    int  bttn            = 0;
    for(;;){
      bttn = gpio_get_level(PIN_PWR_BUTTON);

      if(bttn){
        bttnUnpressCnt++;
        bttnPressCnt = 0;
      }else{
        bttnUnpressCnt = 0;
        bttnPressCnt++;   
        if(bttnPressCnt == PRESS_BTTN_TIME){         
          sPack.cmd = EVENT_BTTN_PRESS;
          xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);
        }
        if(bttnPressCnt == PRESS_BTTN_OFF_ESP_TIME){         
          sPack.cmd = EVENT_BTTN_LONG_PRESS;
          xQueueSend(QueuePwrAut, &sPack, portMAX_DELAY);
        }                  
      }      
      vTaskDelay(10/portTICK_PERIOD_MS);
    }  
}