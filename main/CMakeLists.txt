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

            
)

idf_component_register(SRCS ${SOURCES}
                    # REQUIRES esp_driver_gpio esp_driver_uart nvs_flash esp_adc  bt

                    # INCLUDE_DIRS .  ../include
                    )
