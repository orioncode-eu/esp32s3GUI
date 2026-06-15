/**
 * @file bsp_onx_lvgl.c
 * @brief esp32s3GUI – integracja ONX3248G035 z esp_lvgl_port.
 *
 * Konfiguracja ODTWORZONA 1:1 z działającego HEOStation_Sonnet: task LVGL
 * esp_lvgl_port NIE jest jawnie przypięty do rdzenia (task_affinity = -1 z
 * ESP_LVGL_PORT_INIT_CONFIG()). ui_task siedzi na rdzeniu 0 (core_pins.h),
 * a render/flush LVGL biega tam, gdzie scheduler go postawi.
 *
 * EKSPERYMENT (oba taski graficzne na rdzeniu 0): dopisz przed lvgl_port_init()
 *   lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
 *   port_cfg.task_affinity = GUI_CORE_UI;   // wymaga #include "core_pins.h"
 * i podaj &port_cfg do lvgl_port_init(). Trzymane poza domyślną ścieżką, bo
 * to jedyna realna różnica vs HEOStation przy diagnostyce dotyku.
 *
 * Strategia bufora: partial buffer w PSRAM (8 MB dostępne na ONX3248G035),
 * double-buffer. Bufor = W*H/2 px (= 480*320/2 = 76 800 px ≈ 160 linii), 2×
 * przy double_buffer=true → ~307 KB w PSRAM. Wysoki bufor jest KLUCZOWY dla
 * gładkiego przewijania karuzeli: kafel (~120 px) i jego obwódka mieszczą się
 * w JEDNYM przebiegu render→flush, więc nie widać rysowania paskami.
 */

#include "bsp_onx_lvgl.h"
#include "bsp_onx_panel.h"
#include "bsp_onx_touch.h"
#include "bsp_onx_pinmap.h"
#include "esp_lvgl_port.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "bsp_onx_lvgl";

static lv_display_t *s_disp        = NULL;
static lv_indev_t   *s_touch_indev = NULL;

esp_err_t bsp_onx_lvgl_register(void)
{
    if (s_disp != NULL) return ESP_OK;

    /* 1. LVGL port init (tworzy task LVGL z domyślnymi parametrami; task_affinity
     *    = -1 → nieprzypięty, dokładnie jak w HEOStation). */
    const lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_RETURN_ON_ERROR(lvgl_port_init(&port_cfg), TAG, "LVGL port init failed");

    /* 2. Display – panel ST7796U + IO przez SPI. */
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle     = bsp_onx_panel_io_handle(),
        .panel_handle  = bsp_onx_panel_handle(),
        .buffer_size   = BSP_ONX_DISP_HRES * BSP_ONX_DISP_VRES / 2,
        .double_buffer = true,
        .hres          = BSP_ONX_DISP_HRES,
        .vres          = BSP_ONX_DISP_VRES,
        .monochrome    = false,
        /* Orientacja z bsp_onx_pinmap.h (landscape natywnie / portret w teście).
         * Landscape: MADCTL = MV+MX+MY = 0xE0 (swap_xy=1). Portret: swap_xy=0,
         * wymiary 320×480. (BGR bit wyczyszczony – używamy swap_bytes.)
         * swap_xy  → bit MV: zamienia osie (portrait↔landscape).
         * mirror_x → bit MX: kierunek pionowy (góra/dół).
         * mirror_y → bit MY: kierunek poziomy (lewo/prawo). */
        .rotation      = { .swap_xy  = BSP_ONX_DISP_SWAP_XY,
                           .mirror_x = BSP_ONX_DISP_MIRROR_X,
                           .mirror_y = BSP_ONX_DISP_MIRROR_Y },
        .flags = {
            .buff_dma     = 0,
            .buff_spiram  = 1,    /* PSRAM – 8 MB dostępne na ONX3248G035 */
            .sw_rotate    = 0,
            /* ESP32 jest little-endian: piksele RGB565 w pamięci mają
             * bajt niski (B+G) na adresie 0, wysyłany przez SPI jako pierwszy.
             * Display oczekuje bajtu wysokiego (R+G) jako pierwszego.
             * swap_bytes=1 wywołuje lv_draw_sw_rgb565_swap() przed flush()
             * i koryguje kolejność, usuwając kolorowy fringing na tekście. */
            .swap_bytes   = 1,
            .full_refresh = 0,
            .direct_mode  = 0,
        },
    };
    s_disp = lvgl_port_add_disp(&disp_cfg);
    ESP_RETURN_ON_FALSE(s_disp, ESP_FAIL, TAG, "lvgl_port_add_disp failed");

    /* 3. Touch indev. esp_lvgl_port samo ustawia typ POINTER, read_cb, display
     *    oraz tryb odczytu (TIMER, bo int_gpio_num == GPIO_NUM_NC). */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp   = s_disp,
        .handle = bsp_onx_touch_handle(),
    };
    s_touch_indev = lvgl_port_add_touch(&touch_cfg);
    ESP_RETURN_ON_FALSE(s_touch_indev, ESP_FAIL, TAG, "lvgl_port_add_touch failed");

    ESP_LOGI(TAG, "LVGL port ready (display %dx%d + CST826 touch, LVGL task unpinned)",
             BSP_ONX_DISP_HRES, BSP_ONX_DISP_VRES);
    return ESP_OK;
}
