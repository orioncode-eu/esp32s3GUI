/**
 * @file bsp_onx_bus.h
 * @brief esp32s3GUI – init magistrali I2C dla ONX3248G035.
 *
 * I2C jest wspólny dla PCF8574 (port-expander) i CST826 (touch).
 * Singleton w BSS – po pierwszym init kolejne wywołania są no-op.
 */

#ifndef ESP32S3GUI_BSP_ONX_BUS_H
#define ESP32S3GUI_BSP_ONX_BUS_H

#include "esp_err.h"
#include "driver/i2c_master.h"

esp_err_t bsp_onx_bus_init(void);

/** @brief Zwraca uchwyt magistrali I2C zainicjalizowanej przez bsp_onx_bus_init().
 *         NULL jeśli init jeszcze nie zakończył się sukcesem. */
i2c_master_bus_handle_t bsp_onx_bus_i2c_handle(void);

#endif /* ESP32S3GUI_BSP_ONX_BUS_H */
