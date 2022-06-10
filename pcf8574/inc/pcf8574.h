/*
 * pcf8574.h
 *
 *  Created on: Jun 5, 2022
 *      Author: gianfranco
 */

#ifndef PCF8574_INC_PCF8574_H_
#define PCF8574_INC_PCF8574_H_

#include <stdint.h>


/**
  * @brief PCF8574 status enumeration
  */
typedef enum {
	PCF8574_OK,
	PCF8574_ERROR,
} pcf8574_status;

/**
  * @brief PCF8574 pins enumeration
  */
typedef enum {
	PCF8574_PIN_0,
	PCF8574_PIN_1,
	PCF8574_PIN_2,
	PCF8574_PIN_3,
	PCF8574_PIN_4,
	PCF8574_PIN_5,
	PCF8574_PIN_6,
	PCF8574_PIN_7,

	PCF8574_PIN_COUNT
} pcf8574_pin;

/**
  * @brief PCF8574 pin directions enumeration
  */
typedef enum {
	PCF8574_PIN_INPUT,
	PCF8574_PIN_OUTPUT
} pcf8574_pin_direction;

/**
  * @brief PCF8574 pin status enumeration
  */
typedef enum {
	PCF8574_PIN_RESET,
	PCF8574_PIN_SET
} pcf8574_pin_state;

/**
  * @brief PCF8574 configuration structure definition
  */
typedef struct {
	pcf8574_pin_state pin_states[PCF8574_PIN_COUNT];
	pcf8574_pin_direction pin_directions[PCF8574_PIN_COUNT];
} pcf8574_config;


pcf8574_status PCF8574_init(pcf8574_config *config);

pcf8574_status PCF8574_pin_write(pcf8574_pin pin, pcf8574_pin_state pin_state);

pcf8574_pin_state PCF8574_pin_read(pcf8574_pin pin);

void PCF8574_interrupt_hook(void);

void PCF8574_rx_transfer_completed_hook(void);

void PCF8574_tx_transfer_completed_hook(void);

#endif /* PCF8574_INC_PCF8574_H_ */
