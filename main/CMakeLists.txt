set(SOURCES main.cpp
            AN_taskUsb.cpp
            AN_objects.cpp
            AN_serial.cpp
            JammerState.cpp
            FastCRCsw.cpp
            FastCRChw.cpp
            AN_serialConv.cpp
            AN_shiftDataArr.cpp
            AN_taskRmReceive.cpp
            AN_taskRs485Receive.cpp
            AN_taskRs485Poll.cpp
            AN_taskRs485Send.cpp
            AN_rs485.cpp
            AN_taskAnalog.cpp
            AN_taskButton.cpp
            AN_taskRs485Poll.cpp
            AN_taskMonitor.cpp
            AN_taskLeds.cpp  
            AN_taskPwrAut.cpp
            AN_taskCmd.cpp
            AN_taskBt.cpp
            AN_taskRmAut.cpp
            AN_taskPrefs.cpp

)

idf_component_register(SRCS ${SOURCES}
                    # REQUIRES esp_driver_gpio esp_driver_uart nvs_flash esp_adc  bt

                    INCLUDE_DIRS .  ../include
                    )
