/**
 * @file main.c
 * @brief esp32s3GUI – punkt wejścia (test GUI – lista wyboru).
 */

#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "core_pins.h"
#include "ui_task.h"

static const char *TAG = "main";

#define STACK_UI  8192

void app_main(void)
{
    ESP_LOGI(TAG, "=== esp32s3GUI booting ===");

    /* ui_task: jedyny task aplikacji na rdzeniu UI (core 0).
     * Task LVGL (esp_lvgl_port) NIE jest jawnie przypięty (jak w HEOStation) –
     * patrz bsp_onx_lvgl.c (eksperyment z przypięciem do GUI_CORE_UI opisany tam).
     * Przyszłe taski benchmarku tworzyć na GUI_CORE_APP (core 1). */
    BaseType_t ok = xTaskCreatePinnedToCore(
        ui_task,
        "ui_task",
        STACK_UI,
        NULL,
        5,
        NULL,
        GUI_CORE_UI);

    if (ok != pdPASS) {
        ESP_LOGE(TAG, "Failed to create ui_task");
        return;
    }

    ESP_LOGI(TAG, "ui_task started on core %d (GUI_CORE_UI)", GUI_CORE_UI);
}
