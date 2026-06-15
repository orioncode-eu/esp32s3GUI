/**
 * @file fake_speakers.h
 * @brief esp32s3GUI – lista "zmyślonych" głośników (jak HEOSTATION_FAKE_SPEAKERS).
 *
 * Wspólne dane dla ekranów benchmarku (karuzela, lista) – zero zależności od
 * sieci/HEOS. Nazwy są statyczne (BSS), zwracane jako wskaźniki tylko-do-odczytu.
 */

#ifndef ESP32S3GUI_FAKE_SPEAKERS_H
#define ESP32S3GUI_FAKE_SPEAKERS_H

#include <stdint.h>

/** @brief Liczba zmyślonych głośników. */
uint8_t fake_speakers_count(void);

/** @brief Nazwa głośnika o danym indeksie (NULL gdy poza zakresem). */
const char *fake_speakers_name(uint8_t index);

#endif /* ESP32S3GUI_FAKE_SPEAKERS_H */
