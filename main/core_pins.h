/**
 * @file core_pins.h
 * @brief esp32s3GUI – przypisanie rdzeni CPU (jak HEOStation).
 *
 * Rdzeń 0: wyłącznie grafika (ui_task + task LVGL z esp_lvgl_port).
 * Rdzeń 1: przyszła logika benchmarku / dane syntetyczne — nigdy LVGL API.
 */

#ifndef ESP32S3GUI_CORE_PINS_H
#define ESP32S3GUI_CORE_PINS_H

/** @brief Rdzeń dedykowany UI / LVGL / flush panelu. */
#define GUI_CORE_UI   0

/** @brief Rdzeń dla logiki niegraficznej (przyszłe taski benchmarku). */
#define GUI_CORE_APP  1

#endif /* ESP32S3GUI_CORE_PINS_H */
