idf_component_register(SRCS "exio_pcf8574.c"
                       INCLUDE_DIRS "include"
                       REQUIRES "driver" "esp_driver_i2c")
# ESP-IDF v6.0: driver/i2c_master.h przeniesione do komponentu esp_driver_i2c
# (ze starego komponentu "driver"). Dlatego trzeba dopisać REQUIRES.
