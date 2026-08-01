#include "../include/AN_taskAnalog.h"


AN_taskAnalog::AN_taskAnalog(/* args */)
{
}

AN_taskAnalog::~AN_taskAnalog()
{
}

void AN_taskAnalog::run(void *param){
  static bool critPwrLatch = 0;
  static bool fanEn = 0;
  BYTE ledsCode[4] = {0};
  int a24     = 0; 
  int aTemper = 0;
  int a24_d;
  BYTE a24_tmp;
  
    // adc_oneshot_unit_handle_t adc_handle;

    // // Initialize ADC Oneshot Mode Driver on the ADC Unit
    // adc_oneshot_unit_init_cfg_t init_config; 
    // init_config.unit_id = ADC_UNIT_1;
    // init_config.clk_src = ADC_RTC_CLK_SRC_DEFAULT;
    // ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

 
    // adc_oneshot_chan_cfg_t config;
    // config.bitwidth = ADC_BITWIDTH_12;
    // config.atten    = ADC_ATTEN_DB_12;

    // ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_0, &config));
    // ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_3, &config));
    
 
 
  for(;;){
    // if(G_pwrMode)return; //the BT connection mode is enable
 
  
    // ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_0, &a24));
    // ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_3, &aTemper));
    

    // AN_print("ch_1 VAL -> "+std::to_string(a24)+" ;   ch_2 VAL -> "+std::to_string(aTemper));


    if((aTemper < A_TEMPERATURE_ON_FAN)&&(!fanEn)){
      fanEn = 1;
      gpio_set_level(PIN_FAN,1);
    } 

    if((aTemper > A_TEMPERATURE_OFF_FAN)&&(fanEn)){
      fanEn = 0;
      gpio_set_level(PIN_FAN,0);
    }

    if(a24 < A24_CRITICAL_VAL){
        ledsCode[0]=5;
        if(!critPwrLatch){
            if(!G_pwrMode)xQueueSend(QueueLeds, ledsCode, portMAX_DELAY);           
            critPwrLatch = 1;
        }
    }else{
        G_led_ccl_5 = 0;
        critPwrLatch = 0;
        a24_d   = (A24_NORMAL_VAL - A24_CRITICAL_VAL)/5;
        a24_tmp = (BYTE)(5-(A24_NORMAL_VAL - a24)/a24_d);
        
        ledsCode[0] = 4;
        ledsCode[1] = 0;
        for(int i=0; i<a24_tmp;i++)ledsCode[1] |= (1<<i);
        ledsCode[1] &= 0x0F;               
        if(!G_pwrMode)xQueueSend(QueueLeds, ledsCode, portMAX_DELAY);                 
    }





    vTaskDelay(500/portTICK_PERIOD_MS);
  }

}