/**
 * @file bsp_stub.c
 * @brief esp32s3GUI – BSP STUB: no-op init dla buildu bez fizycznego ekranu.
 */

#include "bsp_stub.h"
#include "esp_log.h"

static const char *TAG = "bsp_stub";

esp_err_t bsp_stub_init(void)
{
    ESP_LOGI(TAG, "STUB display – no hardware init");
    return ESP_OK;
}
