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


void PCF8574_PORT_init_pins(void);

pcf8574_status PCF8574_PORT_init_i2c(void);

void PCF8574_PORT_enable_interrupts(void);

void PCF8574_PORT_disable_interrupts(void);

pcf8574_status PCF8574_PORT_write_register(uint8_t *);

void PCF8574_PORT_read_register(uint8_t *data);

#endif /* PCF8574_INC_PCF8574_STM32F429XX_PORT_H_ */
