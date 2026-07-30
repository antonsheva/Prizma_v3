#include <stdio.h>
#include "../include/main.h"



TaskHandle_t Handle_taskUsb           = NULL;
TaskHandle_t Handle_taskRmReceive     = NULL;


AN_taskUsb            taskUsb;
AN_taskRmReceive      taskRmReceive   ;

void initserial(){
  AN_taskUsb usb;
  AN_taskRmReceive rm;

  Serial .begin(115200);
  Serial2.begin(115200, SERIAL_8N1, UART_RS485_RX, UART_RS485_TX, true);
  Serial1.begin(9600, SERIAL_8N1, UART_RM_RX1, UART_RM_TX1); 

  Serial.onReceive(usb.callback);
  Serial1.onReceive(rm.callback);
}

void initPins(){
  pinMode(PIN_PWR_HOLD_DRV, OUTPUT);
  pinMode(PIN_PWR_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RS485_DIR_DRV, OUTPUT);
  pinMode(PIN_FAN, OUTPUT); 

  pinMode(PIN_CN1, INPUT);
  pinMode(PIN_CN2, INPUT);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  
   
  pinMode(PIN_JMMR_ON_DRV_1, OUTPUT);
  pinMode(PIN_JMMR_ON_DRV_2, OUTPUT);

  digitalWrite(PIN_PWR_HOLD_DRV , 1); 
}

extern "C" void app_main(void)
{
  initObjects();
  initPins();
  initserial();


  xTaskCreate(taskUsb.run,      "t1_taskUsb",       1024*8, NULL, tskIDLE_PRIORITY, &Handle_taskUsb);
  xTaskCreate(taskRmReceive.run, "t2_taskUsbEvent", 2048,   NULL, tskIDLE_PRIORITY, &Handle_taskRmReceive);
}
