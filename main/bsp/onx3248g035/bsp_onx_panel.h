/**
 * @file bsp_onx_panel.h
 * @brief esp32s3GUI – init panelu LCD ST7796U na płytce ONX3248G035.
 *
 * Sekwencja inicjalizacji:
 *  1. PCF8574 (port-expander) – wymaga uprzednio zainicjalizowanego I2C.
 *  2. Hardware reset LCD przez pin EXIO6 PCF8574.
 *  3. SPI bus + panel IO + ST7796U.
 *  4. mirror/invert/disp_on.
 *
 * Wymaga aktywnej magistrali I2C (bsp_onx_bus_init()).
 */

#ifndef ESP32S3GUI_BSP_ONX_PANEL_H
#define ESP32S3GUI_BSP_ONX_PANEL_H

#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

esp_err_t bsp_onx_panel_init(void);

/** @brief Uchwyt panelu (do wpięcia w esp_lvgl_port lub ręcznego flush). */
esp_lcd_panel_handle_t bsp_onx_panel_handle(void);

/** @brief Uchwyt panel-IO (potrzebny przez esp_lvgl_port do callbacku flush done). */
esp_lcd_panel_io_handle_t bsp_onx_panel_io_handle(void);

#endif /* ESP32S3GUI_BSP_ONX_PANEL_H */
