/**
 * @file bsp_stub.h
 * @brief esp32s3GUI – BSP STUB: brak fizycznego wyświetlacza.
 *
 * Używany gdy CONFIG_ESP32S3GUI_DISPLAY_STUB=y. Pozwala uruchomić cały firmware
 * bez wyświetlacza – ui_stub.c (warstwa wyżej) wypisuje komendy UI w logach.
 */

#ifndef ESP32S3GUI_BSP_STUB_H
#define ESP32S3GUI_BSP_STUB_H

#include "esp_err.h"

esp_err_t bsp_stub_init(void);

#endif /* ESP32S3GUI_BSP_STUB_H */
