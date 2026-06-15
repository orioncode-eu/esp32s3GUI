# ============================================================
#  Komponent main esp32s3GUI
# ============================================================

set(BSP_SOURCES "bsp/stub/bsp_stub.c")
if(CONFIG_ESP32S3GUI_DISPLAY_STUB)
    set(BSP_SOURCES
        "bsp/stub/bsp_stub.c"
    )
elseif(CONFIG_ESP32S3GUI_DISPLAY_ST7796_480x320)
    set(BSP_SOURCES
        "bsp/onx3248g035/bsp_onx.c"
        "bsp/onx3248g035/bsp_onx_bus.c"
        "bsp/onx3248g035/bsp_onx_panel.c"
        "bsp/onx3248g035/bsp_onx_backlight.c"
        "bsp/onx3248g035/bsp_onx_touch.c"
        "bsp/onx3248g035/bsp_onx_lvgl.c"
    )
else()
    set(BSP_SOURCES
        "bsp/stub/bsp_stub.c"
    )
endif()

set(BSP_INCLUDE_DIRS "bsp")
if(CONFIG_ESP32S3GUI_DISPLAY_STUB)
    list(APPEND BSP_INCLUDE_DIRS "bsp/stub")
elseif(CONFIG_ESP32S3GUI_DISPLAY_ST7796_480x320)
    list(APPEND BSP_INCLUDE_DIRS "bsp/onx3248g035")
else()
    list(APPEND BSP_INCLUDE_DIRS "bsp/stub")
endif()

set(UI_SOURCES
    "ui/stub/ui_stub.c"
)

if(NOT CONFIG_ESP32S3GUI_DISPLAY_STUB)
    list(APPEND UI_SOURCES
        "ui/screens/screen_carousel.c"
        "ui/core/ui_theme.c"
        "ui/core/ui_kit.c"
        "ui/core/ui_carousel.c"
        "ui/core/ui_fps.c"
        "ui/layout/layout.c"
        "ui/data/fake_speakers.c"
    )
endif()

idf_component_register(
    SRCS
        "main.c"
        "tasks/ui_task.c"
        "hal/display_driver.c"
        ${UI_SOURCES}
        ${BSP_SOURCES}
    INCLUDE_DIRS
        "."
        "tasks"
        "hal"
        "ui/screens"
        "ui/core"
        "ui/layout"
        "ui/data"
        "ui/stub"
        ${BSP_INCLUDE_DIRS}
    REQUIRES
        freertos
        esp_timer
)

message(STATUS "[esp32s3gui] CONFIG_ESP32S3GUI_DISPLAY_ST7796_480x320=${CONFIG_ESP32S3GUI_DISPLAY_ST7796_480x320}")
if(CONFIG_ESP32S3GUI_DISPLAY_ST7796_480x320)
    target_link_libraries(${COMPONENT_LIB} PRIVATE
        idf::esp_lcd
        idf::esp_driver_spi
        idf::esp_driver_i2c
        idf::esp_driver_gpio
        idf::esp_driver_ledc
        idf::esp_lcd_st7796u
        idf::esp_lcd_touch_cst826
        idf::esp_lcd_exio_pcf8574
        idf::lvgl__lvgl
        idf::espressif__esp_lvgl_port
        idf::espressif__esp_lcd_touch
    )
endif()
