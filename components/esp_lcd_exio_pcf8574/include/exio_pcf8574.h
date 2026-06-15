/**
 * @file exio_pcf8574.h
 * @brief PCF8574 I2C I/O expander driver for ONX3248G035.
 *
 * The PCF8574 (address 0x38) controls peripheral signals that are not directly
 * connected to ESP32-S3 GPIOs:
 *   EXIO1 – I2S_CTRL    (speaker amplifier enable)
 *   EXIO2 – RTC_INT     (RTC interrupt)
 *   EXIO3 – CHG_N       (battery charger status, active low)
 *   EXIO4 – CAM_PWDN    (camera power down)
 *   EXIO5 – TP_INT      (touch panel interrupt)
 *   EXIO6 – LCD_RST     (LCD reset, active low)
 *   EXIO7 – SDCS        (SD card chip select)
 */

#pragma once

#include "esp_check.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/** I2C speed for PCF8574 (100 kHz, standard mode) */
#define PCF8574_IIC_SPEED      (100000)

/** PCF8574 I2C slave address */
#define PCF8574_ADDR           (0x38)

/** Maximum timeout for I2C read/write operations (ms) */
#define PCF8574_RW_MAX_TIMEOUT (500)

/**
 * @brief PCF8574 pin mapping (1-indexed to match bit positions).
 */
typedef enum {
    EXIO_PCF8574_PIN_I2S_CTRL = 1,  /*!< Speaker amplifier control */
    EXIO_PCF8574_PIN_RTC_INT,        /*!< RTC interrupt */
    EXIO_PCF8574_PIN_CHG_N,          /*!< Battery charge status (active low) */
    EXIO_PCF8574_PIN_CAM_PWDN,       /*!< Camera power down */
    EXIO_PCF8574_PIN_TP_INT,         /*!< Touch panel interrupt */
    EXIO_PCF8574_PIN_LCD_RST,        /*!< LCD reset (active low) */
    EXIO_PCF8574_PIN_SDCS            /*!< SD card chip select */
} exio_pcf8574_pin;

/**
 * @brief Logic level for PCF8574 pins.
 */
typedef enum {
    EXIO_PCF8574_LEVEL_L = 0,  /*!< Logic low  (0 V) */
    EXIO_PCF8574_LEVEL_H,      /*!< Logic high (VCC) */
} exio_pcf8574_level;

/**
 * @brief Initialize the PCF8574 I2C expander.
 *
 * @note Must be called AFTER i2c_new_master_bus() on I2C_NUM_0.
 *       Internally calls i2c_master_get_bus_handle(0).
 *
 * @return ESP_OK on success, ESP_FAIL if I2C device handle creation fails.
 */
esp_err_t exio_pcf8574_init(void);

/**
 * @brief Read the logic level of a specific PCF8574 pin.
 *
 * @param pin   Target pin (exio_pcf8574_pin)
 * @param level Pointer to store the read level
 * @return ESP_OK on success
 */
esp_err_t exio_pcf8574_pin_read(exio_pcf8574_pin pin, exio_pcf8574_level *level);

/**
 * @brief Set the logic level of a specific PCF8574 pin.
 *
 * @param pin   Target pin (exio_pcf8574_pin)
 * @param level Desired logic level
 * @return ESP_OK on success
 */
esp_err_t exio_pcf8574_pin_write(exio_pcf8574_pin pin, exio_pcf8574_level level);

#ifdef __cplusplus
}
#endif
