# CESE Protocolos de Comunicación Sistemas Embebidos

## Implementación de un driver generico basado en interrupciones para PCF8574

### Introducción
Driver genérico para el expansor de E/S PCF8574. Incluye portabilidad para STM32. Expone 
una API para el manejo del expansor similar a las utilizadas para el manejo de GPIOs. Permite
manejar las entradas y salidas del expansor en forma independiente.

### Implementación
El driver esta diseñado para realizar transferencias de envío y recepción por I2C no bloqueantes.
Se organiza en la siguiente estructura de directorios. 

```bash
pcf8574/
├── inc
│   ├── pcf8574_cfg.h
│   ├── pcf8574.h
│   └── pcf8574_stm32f429xx_port.h
└── src
    ├── pcf8574.c
    └── pcf8574_stm32f429xx_port.c
```

### Ejemplo
```C
/* Adding driver public API */
#include "pcf8574.h"

/* Initializing struct with required configuration. Undefined pins remain as input */
pcf8574_config config = {
			.pin_states[PCF8574_PIN_0] = PCF8574_PIN_RESET,
			.pin_directions[PCF8574_PIN_0] = PCF8574_PIN_OUTPUT,

			.pin_directions[PCF8574_PIN_1] = PCF8574_PIN_INPUT,
};

/* Initializing the driver */ 
pcf8574_status init_status = PCF8574_init(&config);

/* Writing a value to a pin */
pcf8574_status write_status = PCF8574_pin_write(PCF8574_PIN_0, PCF8574_PIN_SET);

/* Reading the value of a pin */
pcf8574_pin_state state = PCF8574_pin_read(PCF8574_PIN_1);

```
