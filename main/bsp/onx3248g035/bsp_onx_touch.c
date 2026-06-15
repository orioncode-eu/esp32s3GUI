/**
 * @file bsp_onx_touch.c
 * @brief esp32s3GUI – init CST826 dla ONX3248G035.
 */

#include "bsp_onx_touch.h"
#include "bsp_onx_bus.h"
#include "bsp_onx_pinmap.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_touch_cst826.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "bsp_onx_touch";

static esp_lcd_touch_handle_t s_touch = NULL;

esp_err_t bsp_onx_touch_init(void)
{
    if (s_touch != NULL) return ESP_OK;

    i2c_master_bus_handle_t i2c = bsp_onx_bus_i2c_handle();
    if (i2c == NULL) {
        ESP_LOGE(TAG, "I2C bus not initialized – call bsp_onx_bus_init() first");
        return ESP_ERR_INVALID_STATE;
    }

    /* IO panel dla touch przez I2C */
    esp_lcd_panel_io_handle_t tp_io = NULL;
    const esp_lcd_panel_io_i2c_config_t tp_io_cfg = {
        .dev_addr                    = BSP_ONX_TP_I2C_ADDR,
        .scl_speed_hz                = BSP_ONX_TP_I2C_SPEED_HZ, /* IDF 6.0: wymagane, inaczej 0 → invalid scl frequency */
        .control_phase_bytes         = 1,
        .dc_bit_offset               = 0,
        /* CST826 ma 8-bitowe adresy rejestrów. lcd_cmd_bits MUSI być 8 –
         * przy 16 adres szedł jako 2 bajty (np. 0xAA→0x00,0xAA), chip lądował
         * na złym rejestrze i zwracał 0x00 (martwy dotyk, chip ID=0x00).
         * Potwierdzone z przykładem producenta (01_touch_test/touch_test.c). */
        .lcd_cmd_bits                = 8,
        .lcd_param_bits              = 8,
        .flags.disable_control_phase = 1,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c(i2c, &tp_io_cfg, &tp_io),
                        TAG, "Touch panel IO create failed");

    const esp_lcd_touch_config_t tp_cfg = {
        .x_max        = BSP_ONX_DISP_HRES,
        .y_max        = BSP_ONX_DISP_VRES,
        .rst_gpio_num = GPIO_NUM_NC,   /* reset przez PCF8574 EXIO */
        .int_gpio_num = GPIO_NUM_NC,   /* INT  przez PCF8574 EXIO  */
        /* Dotyk MUSI mieć tę samą transformację co panel (te same stałe
         * BSP_ONX_DISP_* z bsp_onx_pinmap.h), inaczej osie są zamienione
         * (poziomy gest → pionowy, tapnięcia chybiają). CST826 raportuje w
         * natywnej ramce panelu (portret 320×480); swap_xy=1 daje landscape,
         * swap_xy=0 zostawia portret. x_max/y_max = docelowa rozdzielczość.
         * UWAGA: gdyby oś X lub Y była odwrócona, przełącz MIRROR_X/MIRROR_Y
         * w bsp_onx_pinmap.h (wspólne dla obrazu i dotyku). */
        .flags        = { .swap_xy  = BSP_ONX_DISP_SWAP_XY,
                          .mirror_x = BSP_ONX_DISP_MIRROR_X,
                          .mirror_y = BSP_ONX_DISP_MIRROR_Y },
    };
    ESP_RETURN_ON_ERROR(esp_lcd_touch_new_i2c_cst826(tp_io, &tp_cfg, &s_touch),
                        TAG, "Touch init failed");

    ESP_LOGI(TAG, "CST826 touch ready (addr=0x%02X, %dx%d)",
             BSP_ONX_TP_I2C_ADDR, BSP_ONX_DISP_HRES, BSP_ONX_DISP_VRES);
    return ESP_OK;
}

esp_lcd_touch_handle_t bsp_onx_touch_handle(void)
{
    return s_touch;
}
