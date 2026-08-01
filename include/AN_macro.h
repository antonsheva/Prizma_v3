#ifndef _AN_MACRO_
#define _AN_MACRO_

#define M_RESET_ARR_2(arr,arrLen,elemLen) 	for(int i=0; i<arrLen; i++)memset(arr[i], 0, elemLen)
#define JMMR_1_ON   gpio_set_level(PIN_JMMR_ON_DRV_1, 1);
#define JMMR_2_ON   gpio_set_level(PIN_JMMR_ON_DRV_2, 1);

#define JMMR_1_OFF  gpio_set_level(PIN_JMMR_ON_DRV_1, 0);
#define JMMR_2_OFF  gpio_set_level(PIN_JMMR_ON_DRV_2, 0);


#endif