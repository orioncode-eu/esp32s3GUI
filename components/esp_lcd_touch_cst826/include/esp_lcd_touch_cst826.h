/**
 * @file esp_lcd_touch_cst826.h
 * @brief ESP LCD touch: CST826
 */

#pragma once

#include "esp_lcd_touch.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief I2C address of the CST826 touch controller.
 */
#define ESP_LCD_TOUCH_IO_I2C_CST826_ADDRESS  (0x15)

/**
 * @brief Create a new CST826 touch driver.
 *
 * @note The I2C communication should be initialized before calling this function.
 *
 * @param[in]  io        LCD panel IO handle (created by esp_lcd_new_panel_io_i2c())
 * @param[in]  config    Touch panel configuration
 * @param[out] out_touch Touch panel handle
 * @return
 *   - ESP_OK on success
 */
esp_err_t esp_lcd_touch_new_i2c_cst826(const esp_lcd_panel_io_handle_t io,
                                        const esp_lcd_touch_config_t *config,
                                        esp_lcd_touch_handle_t *out_touch);

#ifdef __cplusplus
}
#endif
