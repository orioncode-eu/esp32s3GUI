/**
 * @file display_driver.h
 * @brief esp32s3GUI – abstrakcja inicjalizacji wyświetlacza.
 *
 * Wybór wyświetlacza następuje w Kconfig (compile-time).
 * W trybie STUB funkcja display_driver_init() jest no-op.
 *
 * Warstwa wyżej (ui_task) wywołuje TYLKO display_driver_init() –
 * całość konfiguracji pinów, SPI, LVGL port jest tutaj.
 */

#ifndef ESP32S3GUI_DISPLAY_DRIVER_H
#define ESP32S3GUI_DISPLAY_DRIVER_H

#include "esp_err.h"

/**
 * @brief Inicjalizuje wyświetlacz i rejestruje go w LVGL przez esp_lvgl_port.
 *        W trybie STUB: no-op, zwraca ESP_OK.
 */
esp_err_t display_driver_init(void);

#endif /* ESP32S3GUI_DISPLAY_DRIVER_H */
