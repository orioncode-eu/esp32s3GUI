/**
 * @file display_driver.c
 * @brief esp32s3GUI – cienki dispatcher wyboru BSP wyświetlacza.
 */

#include "display_driver.h"
#include "esp_log.h"
#include "sdkconfig.h"

#if defined(CONFIG_ESP32S3GUI_DISPLAY_STUB)
    #include "bsp_stub.h"
#elif defined(CONFIG_ESP32S3GUI_DISPLAY_ST7796_480x320)
    #include "bsp_onx.h"
#endif

static const char *TAG = "display_driver";

esp_err_t display_driver_init(void)
{
    ESP_LOGI(TAG, "display_driver_init: dispatching to BSP");

#if defined(CONFIG_ESP32S3GUI_DISPLAY_STUB)
    return bsp_stub_init();

#elif defined(CONFIG_ESP32S3GUI_DISPLAY_ST7796_480x320)
    return bsp_onx_init();

#elif defined(CONFIG_ESP32S3GUI_DISPLAY_ILI9341_320x240) || \
      defined(CONFIG_ESP32S3GUI_DISPLAY_ST7789_240x240)  || \
      defined(CONFIG_ESP32S3GUI_DISPLAY_GC9A01_240x240)
    ESP_LOGW(TAG, "Selected display BSP not implemented yet – running as STUB");
    return ESP_OK;

#else
    ESP_LOGW(TAG, "No display selected in Kconfig – running as STUB");
    return ESP_OK;
#endif
}
