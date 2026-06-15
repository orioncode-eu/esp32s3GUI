/**
 * @file bsp_onx.c
 * @brief esp32s3GUI – orchestracja BSP dla ONX3248G035.
 *
 * Kolejność jest istotna: I2C → panel (z reset przez PCF8574) → backlight →
 * touch (używa I2C) → LVGL port (używa panelu + touch). Każdy krok
 * jest wyizolowany w osobnym pliku, tutaj tylko składamy całość.
 */

#include "bsp_onx.h"
#include "bsp_onx_bus.h"
#include "bsp_onx_panel.h"
#include "bsp_onx_backlight.h"
#include "bsp_onx_touch.h"
#include "bsp_onx_lvgl.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "bsp_onx";

esp_err_t bsp_onx_init(void)
{
    ESP_LOGI(TAG, "ONX3248G035 init: starting...");

    ESP_RETURN_ON_ERROR(bsp_onx_bus_init(),       TAG, "bus init");
    ESP_RETURN_ON_ERROR(bsp_onx_panel_init(),     TAG, "panel init");
    ESP_RETURN_ON_ERROR(bsp_onx_backlight_init(), TAG, "backlight init");
    ESP_RETURN_ON_ERROR(bsp_onx_touch_init(),     TAG, "touch init");
    ESP_RETURN_ON_ERROR(bsp_onx_lvgl_register(),  TAG, "lvgl register");

    ESP_LOGI(TAG, "ONX3248G035 init: OK");
    return ESP_OK;
}
