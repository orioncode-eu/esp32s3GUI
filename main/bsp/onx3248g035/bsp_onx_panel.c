/**
 * @file bsp_onx_panel.c
 * @brief esp32s3GUI – init panelu LCD ST7796U dla ONX3248G035.
 *
 * Reset LCD odbywa się przez pin EXIO6 PCF8574, dlatego I2C MUSI być
 * zainicjalizowany przed wywołaniem bsp_onx_panel_init().
 */

#include "bsp_onx_panel.h"
#include "bsp_onx_pinmap.h"
#include "driver/spi_common.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_st7796u.h"
#include "exio_pcf8574.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "bsp_onx_panel";

static esp_lcd_panel_handle_t    s_panel  = NULL;
static esp_lcd_panel_io_handle_t s_lcd_io = NULL;

static esp_err_t hardware_reset_via_pcf8574(void)
{
    /* PCF8574 init pobiera uchwyt I2C bus 0 przez i2c_master_get_bus_handle(0).
     * Musi być wołane PO i2c_new_master_bus() z portem I2C_NUM_0. */
    ESP_RETURN_ON_ERROR(exio_pcf8574_init(), TAG, "PCF8574 init failed");

    exio_pcf8574_pin_write(EXIO_PCF8574_PIN_LCD_RST, EXIO_PCF8574_LEVEL_L);
    vTaskDelay(pdMS_TO_TICKS(100));
    exio_pcf8574_pin_write(EXIO_PCF8574_PIN_LCD_RST, EXIO_PCF8574_LEVEL_H);
    vTaskDelay(pdMS_TO_TICKS(100));
    return ESP_OK;
}

static esp_err_t spi_bus_create(void)
{
    const spi_bus_config_t cfg = {
        .sclk_io_num     = BSP_ONX_LCD_SCLK,
        .mosi_io_num     = BSP_ONX_LCD_MOSI,
        .miso_io_num     = -1,
        .quadhd_io_num   = -1,
        .quadwp_io_num   = -1,
        /* 16bpp = 2 B/px → *2; /8 to partial buffer fraction (1/8 ramki) */
        .max_transfer_sz = BSP_ONX_LCD_H_RES * BSP_ONX_LCD_V_RES * 2 / 8,
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(BSP_ONX_SPI_HOST, &cfg, SPI_DMA_CH_AUTO),
                        TAG, "SPI bus init failed");
    return ESP_OK;
}

static esp_err_t panel_io_create(void)
{
    const esp_lcd_panel_io_spi_config_t cfg = ST7796U_PANEL_IO_SPI_CONFIG(
        BSP_ONX_LCD_PIXEL_CLK, BSP_ONX_LCD_CS, BSP_ONX_LCD_DC, NULL, NULL);
    ESP_RETURN_ON_ERROR(
        esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)BSP_ONX_SPI_HOST, &cfg, &s_lcd_io),
        TAG, "LCD panel IO create failed");
    return ESP_OK;
}

static esp_err_t panel_create_and_configure(void)
{
    const esp_lcd_panel_dev_config_t cfg = {
        .reset_gpio_num = GPIO_NUM_NC,                 /* reset przez PCF8574 */
        /* RGB (nie BGR): swap_bytes w lvgl_port już koryguje kolejność bajtów SPI,
         * dodatkowy bit BGR w MADCTL powodowałby podwójne odwrócenie R↔B. */
        .rgb_ele_order  = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_st7796u(s_lcd_io, &cfg, &s_panel),
                        TAG, "LCD panel create failed");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(s_panel),               TAG, "LCD reset failed");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(s_panel),                TAG, "LCD init failed");
    /* swap_xy / mirror NIE są tu ustawiane – esp_lvgl_port woła
     * lvgl_port_disp_rotation_update() podczas lvgl_port_add_disp()
     * i nadpisuje MADCTL wartościami z pola rotation w lvgl_port_display_cfg_t.
     * Orientację landscape konfiguruje bsp_onx_lvgl.c: swap_xy=1, mirror_x=1. */
    ESP_RETURN_ON_ERROR(esp_lcd_panel_invert_color(s_panel, false),  TAG, "LCD invert failed");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_disp_on_off(s_panel, true),    TAG, "LCD disp on failed");
    return ESP_OK;
}

esp_err_t bsp_onx_panel_init(void)
{
    if (s_panel != NULL) return ESP_OK;

    ESP_RETURN_ON_ERROR(hardware_reset_via_pcf8574(), TAG, "HW reset failed");
    ESP_RETURN_ON_ERROR(spi_bus_create(),             TAG, "spi_bus_create failed");
    ESP_RETURN_ON_ERROR(panel_io_create(),            TAG, "panel_io_create failed");
    ESP_RETURN_ON_ERROR(panel_create_and_configure(), TAG, "panel_create_and_configure failed");

    ESP_LOGI(TAG, "ST7796U panel ready (%dx%d, BGR16, 80MHz)",
             BSP_ONX_LCD_H_RES, BSP_ONX_LCD_V_RES);
    return ESP_OK;
}

esp_lcd_panel_handle_t    bsp_onx_panel_handle(void)    { return s_panel;  }
esp_lcd_panel_io_handle_t bsp_onx_panel_io_handle(void) { return s_lcd_io; }
