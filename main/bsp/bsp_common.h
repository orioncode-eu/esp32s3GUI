/**
 * @file bsp_common.h
 * @brief esp32s3GUI – wspólne typy/makra dla wszystkich BSP (Board Support Packages).
 *
 * Architektura:
 *  - main/hal/display_driver.c jest CIENKIM dispatcherem i wybiera BSP wg Kconfig.
 *  - Każdy BSP w katalogu main/bsp/<board>/ eksportuje tylko jedną funkcję publiczną
 *    bsp_<name>_init() – patrz np. main/bsp/onx3248g035/bsp_onx.h.
 *  - Wewnątrz BSP rozkład odpowiedzialności (jednoodpowiedzialnościowe pliki):
 *      pinmap.h  – wszystkie GPIO/SPI/I2C constants (zero kodu wykonywalnego)
 *      bus.c     – init magistrali (I2C/SPI)
 *      panel.c   – init panelu LCD (z opcjonalnym hardware reset)
 *      backlight.c – LEDC PWM
 *      touch.c   – init touch controllera
 *      lvgl.c    – wpięcie panelu+touch w esp_lvgl_port
 *
 * Plik bsp_common.h trzymamy MINIMALNY – wspólne typy dodajemy tylko gdy
 * naprawdę dwa różne BSP będą czegoś potrzebowały. W pierwszej iteracji
 * wystarczają stałe rozdzielczości z Kconfig (CONFIG_ESP32S3GUI_DISPLAY_*).
 */

#ifndef ESP32S3GUI_BSP_COMMON_H
#define ESP32S3GUI_BSP_COMMON_H

#include "esp_err.h"

#endif /* ESP32S3GUI_BSP_COMMON_H */
