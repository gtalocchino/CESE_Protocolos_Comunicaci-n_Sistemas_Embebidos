/*
 * pcf8574.c
 *
 *  Created on: Jun 5, 2022
 *      Author: gianfranco
 */

#include "pcf8574.h"
#include "pcf8574_stm32f429xx_port.h"

#include <stdbool.h>


typedef struct {
	pcf8574_pin_state pin_states[PCF8574_PIN_COUNT];
	pcf8574_pin_direction pin_directions[PCF8574_PIN_COUNT];
	uint8_t register_tx;
	uint8_t register_rx;
	bool busy;
	bool pin_states_updated;
	bool reset;
} pcf8574_state;


static pcf8574_state status;


static void update_pin_states(void);


pcf8574_status PCF8574_init(pcf8574_config *config) {
	/* Initializing hardware */
	PCF8574_PORT_init_pins();
	PCF8574_PORT_init_i2c();
	PCF8574_PORT_enable_interrupts();

	/* Setting up pin configuration */
	uint8_t register_tx = UINT8_MAX;
	for (pcf8574_pin pin = 0; pin < PCF8574_PIN_COUNT; pin++) {
		status.pin_states[pin] = config->pin_states[pin];
		status.pin_directions[pin] = config->pin_directions[pin];

		if (status.pin_directions[pin] == PCF8574_PIN_OUTPUT
			&& status.pin_states[pin] == PCF8574_PIN_RESET) {
			register_tx &= ~(1u << pin);
		}
	}

	status.register_tx = register_tx;
	status.register_rx = register_tx;

	/* Writing configuration to the device */
	pcf8574_status write_status = PCF8574_PORT_write_register(&status.register_tx);

	return write_status;
}

pcf8574_status PCF8574_pin_write(pcf8574_pin pin, pcf8574_pin_state pin_state) {
	if (status.busy) {
		return PCF8574_ERROR;
	}

	if (status.pin_directions[pin] != PCF8574_PIN_OUTPUT) {
		return PCF8574_ERROR;
	}

	if (!status.pin_states_updated) {
		update_pin_states();
		status.pin_states_updated = true;
	}

	status.pin_states[pin] = pin_state;

	uint8_t register_tx = UINT8_MAX;
	for (pcf8574_pin pin = 0; pin < PCF8574_PIN_COUNT; pin++) {
		if (status.pin_directions[pin] == PCF8574_PIN_OUTPUT
			&& status.pin_states[pin] == PCF8574_PIN_RESET) {
			register_tx &= ~(1u << pin);
		}
	}

	status.register_tx = register_tx;

	pcf8574_status write_status = PCF8574_PORT_write_register(&status.register_tx);

	return write_status;
}

pcf8574_pin_state PCF8574_pin_read(pcf8574_pin pin) {
	if (!status.pin_states_updated) {
		update_pin_states();
		status.pin_states_updated = true;
	}

	return status.pin_states[pin];
}

void PCF8574_interrupt_hook(void) {
	status.busy = true;
	PCF8574_PORT_read_register(&status.register_rx);
}

void PCF8574_rx_transfer_completed_hook(void) {
	status.pin_states_updated = false;
	status.busy = false;
}

void PCF8574_tx_transfer_completed_hook(void) {
	status.busy = false;
}

static void update_pin_states(void) {
	volatile uint8_t receive_buffer = status.register_rx;
	for (pcf8574_pin pin = 0; pin < PCF8574_PIN_COUNT; pin++) {
		if (receive_buffer & (1u << pin)) {
			status.pin_states[pin] = PCF8574_PIN_SET;
		} else {
			status.pin_states[pin] = PCF8574_PIN_RESET;
		}
	}
}
