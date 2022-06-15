/*
 * pcf8574.c
 *
 *  Created on: Jun 5, 2022
 *      Author: gianfranco
 *
 *  Driver documentation:
 *  - https://github.com/gtalocchino/CESE_Protocolos_Comunicacion_Sistemas_Embebidos
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


/* Driver status */
static pcf8574_state status;


static void update_pin_states(void);
static uint8_t get_register_tx(void);


pcf8574_status PCF8574_init(pcf8574_config *config) {
   /* Initializing the hardware */
   PCF8574_PORT_init_pins();
   PCF8574_PORT_init_i2c();
   PCF8574_PORT_enable_interrupts();

   status.reset = true;

   /* Initializing the driver state with the required configuration */
   uint8_t register_tx = UINT8_MAX;
   for (pcf8574_pin pin = 0; pin < PCF8574_PIN_COUNT; pin++) {
      status.pin_states[pin] = config->pin_states[pin];
      status.pin_directions[pin] = config->pin_directions[pin];

      if (status.pin_directions[pin] == PCF8574_PIN_OUTPUT
            && status.pin_states[pin] == PCF8574_PIN_RESET) {
         register_tx &= ~(1u << pin);
      }
   }

   /* Writing the required configuration to the device */
   status.register_tx = register_tx;
   status.busy = true;
   pcf8574_status write_status = PCF8574_PORT_write_register(&status.register_tx);

   return write_status;
}

pcf8574_status PCF8574_pin_write(pcf8574_pin pin, pcf8574_pin_state pin_state) {
   if (status.busy) {
      /* The I2C bus is busy */
      return PCF8574_BUSY;
   }

   if (status.pin_directions[pin] != PCF8574_PIN_OUTPUT) {
      /* Cannot set a value on a pin configured as an input */
      return PCF8574_ERROR;
   }

   if (!status.pin_states_updated) {
      /* Driver status variable is not updated */
      update_pin_states();
   }

   status.pin_states[pin] = pin_state;

   /* Writing the required configuration to the device */
   status.register_tx = get_register_tx();
   status.busy = true;
   pcf8574_status write_status = PCF8574_PORT_write_register(&status.register_tx);

   return write_status;
}

pcf8574_pin_state PCF8574_pin_read(pcf8574_pin pin) {
   if (status.reset) {
      /* The device has not been read after a reset yet */
      return PCF8574_BUSY;
   }

   if (!status.pin_states_updated) {
      update_pin_states();
   }

   return status.pin_states[pin];
}

pcf8574_status PCF8574_pin_reinit(pcf8574_pin pin, pcf8574_pin_direction pin_direction,
                                  pcf8574_pin_state pin_state) {
   if (status.busy) {
      /* The I2C bus is busy */
      return PCF8574_BUSY;
   }

   if (!status.pin_states_updated) {
      update_pin_states();
   }

   status.pin_states[pin] = pin_state;
   status.pin_directions[pin] = pin_direction;

   if (status.pin_directions[pin] == PCF8574_PIN_INPUT) {
      status.reset = true;
   }

   /* Writing the required configuration to the device */
   status.register_tx = get_register_tx();
   status.busy = true;
   pcf8574_status write_status = PCF8574_PORT_write_register(&status.register_tx);

   return write_status;
}

void _PCF8574_interrupt_callback(void) {
   /*
    * The PCF8574 detected a change on some of its pins.
    * This function triggers a read of the device
    */
   status.busy = true;
   PCF8574_PORT_read_register(&status.register_rx);
}

void _PCF8574_rx_transfer_completed_callback(void) {
   /* Device reading is complete. The driver status is updated */
   status.pin_states_updated = false;
   status.busy = false;

   if (status.reset) {
      status.reset = false;
   }
}

void _PCF8574_tx_transfer_completed_callback(void) {
   /* Device writing is complete. The driver status is updated */
   status.busy = false;

   if (status.reset) {
      /* Triggering a device read after a reset */
      status.busy = true;
      PCF8574_PORT_read_register(&status.register_rx);
   }
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

   status.pin_states_updated = true;
}

static uint8_t get_register_tx(void) {
   uint8_t register_tx = UINT8_MAX;
   for (pcf8574_pin pin = 0; pin < PCF8574_PIN_COUNT; pin++) {
      if (status.pin_directions[pin] == PCF8574_PIN_OUTPUT
            && status.pin_states[pin] == PCF8574_PIN_RESET) {
         register_tx &= ~(1u << pin);
      }
   }

   return register_tx;
}
