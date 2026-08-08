#include "../include/AN_taskLeds.h"
 


AN_taskLeds::AN_taskLeds(/* args */)
{
}

AN_taskLeds::~AN_taskLeds()
{
}

void AN_taskLeds::setState(BYTE stt){
  if(stt & (1<<0))gpio_set_level(LED_1, 1);
  else            gpio_set_level(LED_1, 0);

  if(stt & (1<<1))gpio_set_level(LED_2, 1);
  else            gpio_set_level(LED_2, 0);

  if(stt & (1<<2))gpio_set_level(LED_3, 1);
  else            gpio_set_level(LED_3, 0);

  if(stt & (1<<3))gpio_set_level(LED_4, 1);
  else            gpio_set_level(LED_4, 0);

  if(stt & (1<<4))gpio_set_level(LED_5, 1);
  else            gpio_set_level(LED_5, 0);

  if(stt & (1<<5))gpio_set_level(LED_6, 1);
  else            gpio_set_level(LED_6, 0);    
}

void AN_taskLeds::run(void *param){
    BYTE stt = 0;
    bool swch  = 0; 
    BYTE cntTm = 0;
    BYTE sftCnt = 0;
    for(;;){
      switch(G_ledsStste[0]){
        case 0: if(!G_ledsStste[1])setState(0);
                G_ledsStste[1] = 1;
        break;

        case 1: if(!G_ledsStste[1]){
                if(cntTm < 4)cntTm++;
                else{
                    cntTm = 0;
                    setState(1 << (sftCnt < 6 ? sftCnt : sftCnt-6));
                    sftCnt++;
                    if(sftCnt>=12){                   
                    sftCnt = 0;
                    G_ledsStste[1] = 2;
                    cntTm = 0;
                    setState(0);
                    }
                }          
              }
        break;
      
        case 2: if(cntTm < 4)cntTm++;
                else{
                    cntTm = 0;
                    swch = !swch;
                    stt &= 0x30; 
                    stt |= swch ? 0x01 : 0x00;
                    setState(stt);          
                }          
        break; 

        case 3: stt |= (G_voltToLeds & 0xC0);
                setState(G_voltToLeds); break;

        case 4: if(cntTm < 4)cntTm++;
                else{
                    cntTm = 0;
                    swch = !swch;
                    stt &= 0x30; 
                    stt |= swch ? 0x09 : 0x00;
                    setState(stt);
                }          
        break; 
        
        case 5: stt &= 0x30;
                stt |= 0x09;
                setState(stt);
        break;
        case 6: stt &= 0x30;
                stt |= 0x06;
                setState(stt);
        break;   
        case 7: if(!G_ledsStste[1]){
                if(cntTm < 4)cntTm++;
                else{
                    cntTm = 0;
                    stt &= 0x30;
                    if(sftCnt%2)stt |= 0x03;
                    else        stt |= 0x0C;
                    setState(stt);
                    sftCnt++;
                    if(sftCnt>=6){                   
                    sftCnt = 0;
                    G_ledsStste[1] = 8;
                    cntTm = 0;
                    setState(0);
                    }
                }          
              }
        break;
      }

      vTaskDelay(50/portTICK_PERIOD_MS);
    }
}