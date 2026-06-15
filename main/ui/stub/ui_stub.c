/**
 * @file ui_stub.c
 * @brief esp32s3GUI – implementacja stub wyświetlacza.
 */

#include "ui_stub.h"
#include "esp_log.h"

static const char *TAG = "ui_stub";

void ui_stub_init(void)
{
    ESP_LOGI(TAG, "=== UI STUB MODE (no physical display) ===");
}

void ui_stub_show_page(uint8_t page)
{
    ESP_LOGI(TAG, "[SCREEN] To jest ekran nr %u", (unsigned)page);
}
