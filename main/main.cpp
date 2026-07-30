#include <stdio.h>
#include "main.h"


extern "C" void app_main(void)
{

  Serial.begin(115200);
  for(;;){

    Serial.println("dfgdfgdfgd");
    vTaskDelay(300/portTICK_PERIOD_MS);
  }


}
