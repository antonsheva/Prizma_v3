set(SOURCES main.cpp

)

idf_component_register(SRCS ${SOURCES}
                    # REQUIRES esp_driver_gpio esp_driver_uart nvs_flash esp_adc  bt

                    # INCLUDE_DIRS .  ../include
                    )
