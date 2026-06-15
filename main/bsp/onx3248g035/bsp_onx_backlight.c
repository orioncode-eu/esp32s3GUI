/**
 * @file bsp_onx_backlight.c
 * @brief esp32s3GUI – LEDC PWM dla pinu BL na ONX3248G035.
 */

#include "bsp_onx_backlight.h"
#include "bsp_onx_pinmap.h"
#include "driver/ledc.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "bsp_onx_bl";

#define BL_TIMER       LEDC_TIMER_1
#define BL_CHANNEL     LEDC_CHANNEL_0
#define BL_MODE        LEDC_LOW_SPEED_MODE
#define BL_FREQ_HZ     10000
#define BL_DUTY_RES    LEDC_TIMER_10_BIT
#define BL_DUTY_MAX    1023      /* (1<<10)-1 dla 10-bit */

static bool s_inited = false;

esp_err_t bsp_onx_backlight_init(void)
{
    if (s_inited) return ESP_OK;

    const ledc_timer_config_t t = {
        .speed_mode      = BL_MODE,
        .timer_num       = BL_TIMER,
        .duty_resolution = BL_DUTY_RES,
        .freq_hz         = BL_FREQ_HZ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ESP_RETURN_ON_ERROR(ledc_timer_config(&t), TAG, "BL timer failed");

    const ledc_channel_config_t ch = {
        .speed_mode = BL_MODE,
        .channel    = BL_CHANNEL,
        .timer_sel  = BL_TIMER,
        .gpio_num   = BSP_ONX_LCD_BL,
        .duty       = BL_DUTY_MAX,
        .hpoint     = 0,
    };
    ESP_RETURN_ON_ERROR(ledc_channel_config(&ch), TAG, "BL channel failed");

    s_inited = true;
    ESP_LOGI(TAG, "Backlight LEDC PWM ready (GPIO=%d, 10kHz, ~100%%)", BSP_ONX_LCD_BL);
    return ESP_OK;
}

esp_err_t bsp_onx_backlight_set_pct(uint8_t pct)
{
    if (!s_inited) return ESP_ERR_INVALID_STATE;
    if (pct > 100) pct = 100;
    uint32_t duty = (uint32_t)pct * BL_DUTY_MAX / 100u;
    ESP_RETURN_ON_ERROR(ledc_set_duty(BL_MODE, BL_CHANNEL, duty), TAG, "set_duty");
    ESP_RETURN_ON_ERROR(ledc_update_duty(BL_MODE, BL_CHANNEL),    TAG, "update_duty");
    return ESP_OK;
}
