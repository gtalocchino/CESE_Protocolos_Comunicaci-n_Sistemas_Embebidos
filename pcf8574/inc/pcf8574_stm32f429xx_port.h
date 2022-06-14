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
 * @brief Initialize the necessary pins.
 *
 * This function initializes the pins necessary for driver operation.
 * This includes the I2C bus pins and the pin for the INT signal.
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
 * @brief Enables the necessary interrupts for driver operation.
 *
 * This function enables the necessary interrupts for the operation of
 * the driver.
 *
 */
void PCF8574_PORT_enable_interrupts(void);

/*!
 * @brief Writes the control register of the PCF8574 through the I2C bus.
 *
 * This function writes the I2C bus control register via a non-blocking
 * mechanism.
 *
 * @param data Pointer to the data buffer.
 */
pcf8574_status PCF8574_PORT_write_register(uint8_t *);

/*!
 * @brief Reads the control register of the PCF8574 through the I2C bus.
 *
 * This function reads the I2C bus control register via a non-blocking
 * mechanism.
 *
 * @param data Pointer to the data buffer.
 */
void PCF8574_PORT_read_register(uint8_t *data);

#endif /* PCF8574_INC_PCF8574_STM32F429XX_PORT_H_ */
