/**
 * @file fake_speakers.c
 * @brief esp32s3GUI – implementacja listy zmyślonych głośników.
 */

#include "fake_speakers.h"

static const char *const k_names[] = {
    "Salon", "Kuchnia", "Sypialnia", "Lazienka glowna na pietrze",
    "Gabinet", "Pokoj dzieciecy", "Taras", "Garaz",
    "Jadalnia", "Korytarz", "Piwnica", "Strych",
    "Biuro", "Silownia", "Ogrod zimowy", "Pokoj goscinny",
};

#define FAKE_SPEAKERS_COUNT  ((uint8_t)(sizeof(k_names) / sizeof(k_names[0])))

uint8_t fake_speakers_count(void)
{
    return FAKE_SPEAKERS_COUNT;
}

const char *fake_speakers_name(uint8_t index)
{
    if (index >= FAKE_SPEAKERS_COUNT) {
        return NULL;
    }
    return k_names[index];
}
