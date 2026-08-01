#include <stdio.h>
#include "../include/main.h"

 // Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif


TaskHandle_t Handle_taskUsb           = NULL;
TaskHandle_t Handle_taskRmReceive     = NULL;
TaskHandle_t Handle_taskRs485Receive  = NULL;  
TaskHandle_t Handle_taskRs485Poll     = NULL;
TaskHandle_t Handle_taskButton        = NULL;
TaskHandle_t Handle_taskAnalog        = NULL;
TaskHandle_t Handle_taskMonitor       = NULL;

TaskHandle_t Handle_taskLeds          = NULL;
TaskHandle_t Handle_taskPwrAut        = NULL;
TaskHandle_t Handle_taskCmd           = NULL;
TaskHandle_t Handle_taskBt            = NULL;
TaskHandle_t Handle_taskRs485Send     = NULL;
TaskHandle_t Handle_taskRmAut         = NULL;
TaskHandle_t Handle_taskPrefs         = NULL;


AN_taskUsb            taskUsb;
AN_taskRmReceive      taskRmReceive   ;
AN_taskRs485Receive   taskRs485Receive;  
AN_taskRs485Poll      taskRs485Poll   ;
AN_taskRs485Send      taskRs485Send   ;
AN_taskButton         taskButton      ;
AN_taskAnalog         taskAnalog      ;
AN_taskMonitor        taskMonitor     ;
AN_taskLeds           taskLeds        ;
AN_taskPwrAut         taskPwrAut      ;
AN_taskCmd            taskCmd         ;
AN_taskBt             taskBt          ;
AN_taskRmAut          taskRmAut       ;
AN_taskPrefs          taskPrefs       ;

void initserial(){
  AN_taskUsb usb;
  AN_taskRmReceive rm;
  AN_taskRs485Receive rs485; 

  Serial .begin(115200);
  Serial2.begin(115200, SERIAL_8N1, UART_RS485_RX, UART_RS485_TX, true);

  Serial1.setTimeout(100);  
  Serial1.begin(9600, SERIAL_8N1, UART_RM_RX1, UART_RM_TX1); 

  Serial.onReceive (usb.callback);
  Serial1.onReceive(rm.callback);
  Serial2.onReceive(rs485.callback);

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

  xTaskCreate(taskUsb.run,          "t1_taskUsb",          1024*8, NULL, tskIDLE_PRIORITY, &Handle_taskUsb          );
  xTaskCreate(taskRmReceive.run,    "t2_taskUsbEvent",     2048,   NULL, tskIDLE_PRIORITY, &Handle_taskRmReceive    );
  xTaskCreate(taskRs485Poll.run,    "t3_taskRs485Poll",    1024*4, NULL, tskIDLE_PRIORITY, &Handle_taskRs485Poll    );
  xTaskCreate(taskRs485Receive.run, "t4_taskRs485Receive", 1024*8, NULL, tskIDLE_PRIORITY, &Handle_taskRs485Receive );
  xTaskCreate(taskButton.run,       "t5_taskButton",       2048,   NULL, tskIDLE_PRIORITY, &Handle_taskButton       );
  xTaskCreate(taskAnalog.run,       "t6_taskAnalog",       2048,   NULL, tskIDLE_PRIORITY, &Handle_taskAnalog       );
  xTaskCreate(taskMonitor.run,      "t7_taskMonitor",      2048,   NULL, tskIDLE_PRIORITY, &Handle_taskMonitor      );
  xTaskCreate(taskLeds.run,         "t8_taskLeds",         2048,   NULL, tskIDLE_PRIORITY, &Handle_taskLeds         ); 
  xTaskCreate(taskPwrAut.run,       "t9_taskPwrAut",       1024*4, NULL, tskIDLE_PRIORITY, &Handle_taskPwrAut       );    
  xTaskCreate(taskCmd.run,          "t10_taskCmd",         1024*8, NULL, tskIDLE_PRIORITY, &Handle_taskCmd          );    
  xTaskCreate(taskBt.run,           "t11_taskBt",          1024*8, NULL, tskIDLE_PRIORITY, &Handle_taskBt           );
  xTaskCreate(taskRs485Send.run,    "t12_taskRs485Send",   1024*4, NULL, tskIDLE_PRIORITY, &Handle_taskRs485Send    );
  xTaskCreate(taskRmAut.run,        "t13_taskRmAut",       1024*4, NULL, tskIDLE_PRIORITY, &Handle_taskRmAut        );
  xTaskCreate(taskPrefs.run,        "t14_taskPrefs",       1024*2, NULL, tskIDLE_PRIORITY, &Handle_taskPrefs        );  
  
}
