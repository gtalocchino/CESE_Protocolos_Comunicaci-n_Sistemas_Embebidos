/*
 * pcf8574_stm32f429xx_port.h
 *
 *  Created on: Jun 5, 2022
 *      Author: gianfranco
 */

#ifndef PCF8574_INC_PCF8574_STM32F429XX_PORT_H_
#define PCF8574_INC_PCF8574_STM32F429XX_PORT_H_

#include <stdint.h>

#include "pcf8574.h"


/*!
 * @brief Initializes necessary pins.
 *
 * This function initializes necessary pins for driver operation.
 * The required pins are SDA and SCK for the I2C peripheral and an
 * input pin capable of generating an interrupt on a falling edge for INT.
 *
 */
void PCF8574_PORT_init_pins(void);

/*!
 * @brief Initializes the I2C peripheral.
 *
 * This function initializes the I2C peripheral needed for
 * communication with the PCF5874.
 *
 */
pcf8574_status PCF8574_PORT_init_i2c(void);

/*!
 * @brief Initializes the I2C peripheral.
 *
 * This function initializes the I2C peripheral needed for
 * communication with the PCF5874.
 *
 */
void PCF8574_PORT_enable_interrupts(void);

pcf8574_status PCF8574_PORT_write_register(uint8_t *);

void PCF8574_PORT_read_register(uint8_t *data);

#endif /* PCF8574_INC_PCF8574_STM32F429XX_PORT_H_ */
