/**
 * @file ui_task.c
 * @brief esp32s3GUI – UI task: init display + ekran listy wyboru.
 */

#include "ui_task.h"
#include "ui_stub.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

static const char *TAG = "ui_task";

#ifndef CONFIG_ESP32S3GUI_DISPLAY_STUB
    #include "display_driver.h"
    #include "esp_lvgl_port.h"
    #include "screen_list.h"
    #include "ui_theme.h"
    #include "ui_fps.h"
#endif

void ui_task(void *arg)
{
    (void)arg;

    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    ESP_LOGI(TAG, "UI task started");

#ifdef CONFIG_ESP32S3GUI_DISPLAY_STUB
    ESP_LOGI(TAG, "Running in STUB mode");
    ui_stub_init();
    ui_stub_show_page(1);

    while (1) {
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
#else
    ESP_ERROR_CHECK(display_driver_init());
    ui_theme_init();
    ui_fps_init();

    if (lvgl_port_lock(0)) {
        (void)screen_list_load();
        lvgl_port_unlock();
    } else {
        ESP_LOGE(TAG, "Initial lvgl_port_lock failed");
    }

    while (1) {
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
#endif
}
