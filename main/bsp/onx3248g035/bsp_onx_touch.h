/**
 * @file bsp_onx_touch.h
 * @brief esp32s3GUI – CST826 touch przez I2C na ONX3248G035.
 *
 * Wymaga aktywnej magistrali I2C (bsp_onx_bus_init()) oraz – dla spójności
 * wymiarów – znanych BSP_ONX_LCD_H_RES/V_RES.
 */

#ifndef ESP32S3GUI_BSP_ONX_TOUCH_H
#define ESP32S3GUI_BSP_ONX_TOUCH_H

#include "esp_err.h"
#include "esp_lcd_touch.h"

esp_err_t bsp_onx_touch_init(void);

/** @brief Uchwyt touch controllera – do wpięcia w esp_lvgl_port. */
esp_lcd_touch_handle_t bsp_onx_touch_handle(void);

#endif /* ESP32S3GUI_BSP_ONX_TOUCH_H */
