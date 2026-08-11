#ifndef _DEFINE_
#define _DEFINE_


#define PIN_JMMR_ON_DRV_1   GPIO_NUM_15
#define PIN_JMMR_ON_DRV_2   GPIO_NUM_21
#define PIN_FAN             GPIO_NUM_23
#define PIN_RS485_DIR_DRV   GPIO_NUM_4
#define PIN_PWR_HOLD_DRV    GPIO_NUM_22
#define PIN_PWR_BUTTON      GPIO_NUM_19
#define PIN_CN1             GPIO_NUM_18
#define PIN_CN2             GPIO_NUM_5

#define LED_1     GPIO_NUM_13
#define LED_2     GPIO_NUM_12
#define LED_3     GPIO_NUM_14
#define LED_4     GPIO_NUM_27
#define LED_5     GPIO_NUM_26
#define LED_6     GPIO_NUM_25

#define CMD_RM_AT           1             
#define CMD_RM_GET_ATBT     2             
#define CMD_RM_GET_ATC      3             
#define CMD_RM_SET_ATC      4             
#define CMD_RM_SET_ATE0     5              
#define CMD_RM_SET_ATE1     6              
#define CMD_RM_GET_ATI      7             
#define CMD_RM_ATZ          8             
#define CMD_RM_GET_ATW      9             
#define CMD_RM_SET_ATW      10             
#define CMD_RM_GET_STATE    11
#define CMD_SET_JMMR_DATA   12
#define CMD_RM_GET_INFO     13
#define CMD_PRINT_ADDRESSES 14
// #define CMD_SEARCH_DEVICES 15

#define CMD_SET_ADDR_RM_1   16
#define CMD_SET_ADDR_RM_2   17
#define CMD_RESPONSE_DATA   18
#define CMD_GET_JMMR_LIST   19
#define CMD_GET_JMMR_DATA   20

#define CMD_GET_STACK_SIZE  21
#define CMD_GEN_TEST_DATA   22
#define CMD_SET_JMMR_LIST   23
#define CMD_TEST            24     
#define CMD_RESUME_WORK 25
#define CMD_SET_ADDR_ESP    26
#define CMD_SET_PWR         27
#define CMD_RESTART_ESP     28
#define CMD_SET_ADDR_RM     29
#define CMD_SET_DEV_ID      30
#define CMD_SET_DEV_TYPE    31
#define CMD_SET_GROUP_ID    32
#define CMD_SET_DEV_RANGE   33
#define CMD_GET_DEV_PARAM   34
#define CMD_GET_ALL_STACK   35
#define CMD_APLAY_PWR       36
#define CMD_UPDT_LOC_DATA   37
#define CMD_BT_START        38
#define CMD_BT_STOP         39
#define CMD_BT_SEND         40
#define CMD_BT_RECEIVE      41  
#define CMD_PRINT_JMMR_DATA 42  
#define CMD_PRINT_JMMR_LIST 43 



#define PARAM_CMD               "cmd"                            
#define PARAM_MOD_CODE          "mc"                            
#define PARAM_MASK              "msk"                            
#define PARAM_RM_NUM            "rm_num"                            
#define PARAM_RM_STATE          "rm_stt"                            
#define PARAM_SENDER            "sender"                            
#define PARAM_JMMR_LIST         "jmmr_list"                            
#define PARAM_JMMR_LIST_LEN     "jmmr_list_len"                            
#define PARAM_RESPONSE          "resp"                            
#define PARAM_MSG_DIR           "dir"                            
#define PARAM_TASK_ID           "task_id"                                     
#define PARAM_DEV_ID            "dev_id"                                                
#define PARAM_GROUP_ID          "group_id"             
#define PARAM_DEV_TYPE          "dev_type"              
#define PARAM_DEV_RANGE         "dev_range"           
#define PARAM_ADDR_ESP          "ad_esp"          
#define PARAM_ADDR_RM_1         "ad_rm1"                    
#define PARAM_ADDR_RM_2         "ad_rm2"          
#define PARAM_MOD_CODE_1        "mc1"                                      
#define PARAM_MOD_CODE_2        "mc2"                                      
#define PARAM_MASK_1            "msk1"                                      
#define PARAM_MASK_2            "msk2"                                      
#define PARAM_PWR_1             "pwr1"                                      
#define PARAM_PWR_2             "pwr2"                                      
#define PARAM_TXT               "txt"                                                
#define PARAM_TXT_LEN           "txt_len"                              
#define PARAM_BATT_STATE        "batt_stt"                              
#define PARAM_TEMPERATURE       "temper"  
#define PARAM_NEED_BT_OFF       "need_bt_off"  
#define PARAM_NEED_ESP_RESTART  "need_bt_restart"  
#define PARAM_ADDRESSEE         "addressee" 
                                                         


#define RESP_OK             1
#define RESP_ERROR          2
#define RESP_SEARCH_DEVICES 3
#define RESP_GET_JMMR_DATA  4
#define RESP_SET_JMMR_DATA  5
#define RESP_GET_JMMR_LIST  6
#define RESP_SET_JMMR_LIST  7

 
#define PWR_ON  1
#define PWR_OFF 2
#define BT_ON  1
 
#define PWR_MODE_START_ESP          0
#define PWR_MODE_MAIN               1
#define PWR_MODE_BT_WAIT_CONNECT    2
#define PWR_MODE_BT_CONNECT         3
#define PWR_MODE_PWR_OFF            4
#define PWR_MODE_APPLY_CHANGE       5
#define PWR_MODE_RESTART            6


#define MSG_DIR_REQUEST  0
#define MSG_DIR_RESPONSE 1

#define MAX_SERIAL_DATA_LEN 4096
#define RS485_BUFF_LEN      1024
#define RM_BUFF_LEN         512
#define SERIAL_BUFF_LEN     128
#define TXT_BUFF_LEN        512
#define TXT_INFO_LEN        TXT_BUFF_LEN-128

#define MAX_STR_LEN         64


#define SRC_COM 0
#define SRC_BT  1

#define MAX_DEVICE_QTY      10



#define SERIAL_SRC_RM        0
#define SERIAL_SRC_USB       1
#define SERIAL_SRC_485       2
#define SERIAL_SRC_BT        3

#define EVENT_BTTN_PRESS            1
#define EVENT_BTTN_LONG_PRESS       2
#define EVENT_BT_CONNECT            3
#define EVENT_BT_DISCONNECT         4
#define EVENT_TIMEOUT_BT_CONNECT    5
#define EVENT_APPLY_CHANGES         6
#define EVENT_RESTART_ESP           7
#define EVENT_RESUME_WORK           8

#define A24_CRITICAL_VAL      2200
#define A24_NORMAL_VAL        2800

#define A_TEMPERATURE_ON_FAN  2580
#define A_TEMPERATURE_OFF_FAN 2620

#define DEV_TYPE_A   1
#define DEV_TYPE_B   2



#define UART_BUF_SIZE 1024

#define UART_USB      UART_NUM_0
#define UART_USB_TX   GPIO_NUM_1
#define UART_USB_RX   GPIO_NUM_3 

#define UART_RS485    UART_NUM_2  
#define UART_RS485_TX GPIO_NUM_17
#define UART_RS485_RX GPIO_NUM_16

#define UART_RM        UART_NUM_1  

#define UART_RM_TX1    GPIO_NUM_32
#define UART_RM_RX1    GPIO_NUM_34

#define UART_RM_TX2    GPIO_NUM_33
#define UART_RM_RX2    GPIO_NUM_35



#endif