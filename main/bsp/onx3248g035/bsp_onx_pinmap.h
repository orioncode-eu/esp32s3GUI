/**
 * @file bsp_onx_pinmap.h
 * @brief esp32s3GUI – mapa GPIO/SPI/I2C dla płytki ONX3248G035 (Nextion 3.5").
 *
 * Stałe wynikają ze schematu producenta (OpenNextion-SKU-ONX3248G035) i są
 * niezmienne dla tej płytki. Plik nie zawiera kodu wykonywalnego.
 *
 * Hardware:
 *  - LCD: ST7796U 480x320 (SPI, 80 MHz)
 *  - Touch: CST826 (I2C addr 0x15)
 *  - I/O expander: PCF8574 (I2C, steruje m.in. RST LCD przez EXIO6)
 */

#ifndef ESP32S3GUI_BSP_ONX_PINMAP_H
#define ESP32S3GUI_BSP_ONX_PINMAP_H

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/i2c_master.h"

/* ── Rozdzielczość (natywna płytka: landscape 480×320) ─────────────── */
#define BSP_ONX_LCD_H_RES      480
#define BSP_ONX_LCD_V_RES      320

/* ── Orientacja efektywna (test) ──────────────────────────────────────
 * Domyślnie landscape (jak natywnie). Opcja Kconfig ESP32S3GUI_FORCE_PORTRAIT
 * (menu "Development / Test") przełącza na portret: zamieniamy wymiary
 * (320×480) i NIE zamieniamy osi (swap_xy=0) – panel skanuje natywnie.
 * Display (bsp_onx_lvgl.c) i touch (bsp_onx_touch.c) używają TYCH SAMYCH
 * stałych, więc dotyk podąża za obrazem.
 * Portret: mx=1,my=0 (USB = dół, nagłówek u góry, tekst nie lustrzany).
 * Landscape: swap_xy=1, mx=my=1. */
#if CONFIG_ESP32S3GUI_FORCE_PORTRAIT
#  define BSP_ONX_DISP_HRES     BSP_ONX_LCD_V_RES   /* 320 */
#  define BSP_ONX_DISP_VRES     BSP_ONX_LCD_H_RES   /* 480 */
#  define BSP_ONX_DISP_SWAP_XY  0
   /* Kalibracja montażu panelu (USB = dół obudowy):
    * mx=my=1 → do góry nogami, tekst OK
    * mx=my=0 → orientacja OK, tekst lustrzany (odbicie poziome)
    * mx=1,my=0 → orientacja OK + tekst czytelny (tylko MY odwrócone vs start).
    * Dotyk: te same flagi co display. */
#  define BSP_ONX_DISP_MIRROR_X 1
#  define BSP_ONX_DISP_MIRROR_Y 0
#else
#  define BSP_ONX_DISP_HRES     BSP_ONX_LCD_H_RES   /* 480 */
#  define BSP_ONX_DISP_VRES     BSP_ONX_LCD_V_RES   /* 320 */
#  define BSP_ONX_DISP_SWAP_XY  1
#  define BSP_ONX_DISP_MIRROR_X 1
#  define BSP_ONX_DISP_MIRROR_Y 1
#endif

/* ── SPI (LCD) ────────────────────────────────────────────────────── */
#define BSP_ONX_SPI_HOST       SPI2_HOST
#define BSP_ONX_LCD_SCLK       GPIO_NUM_5
#define BSP_ONX_LCD_MOSI       GPIO_NUM_1
#define BSP_ONX_LCD_DC         GPIO_NUM_3
#define BSP_ONX_LCD_CS         GPIO_NUM_2
#define BSP_ONX_LCD_BL         GPIO_NUM_6   /* Backlight – LEDC PWM     */
#define BSP_ONX_LCD_PIXEL_CLK  (80 * 1000 * 1000) /* 80 MHz             */

/* ── I2C (Touch + PCF8574) ────────────────────────────────────────── */
#define BSP_ONX_I2C_PORT       I2C_NUM_0
#define BSP_ONX_I2C_SCL        GPIO_NUM_7
#define BSP_ONX_I2C_SDA        GPIO_NUM_8
#define BSP_ONX_TP_I2C_ADDR      (0x15)    /* CST826 I2C address       */
#define BSP_ONX_TP_I2C_SPEED_HZ  (400000)  /* Fast-mode 400 kHz; wymagane w IDF 6.0 */

#endif /* ESP32S3GUI_BSP_ONX_PINMAP_H */
