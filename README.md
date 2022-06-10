# CESE Protocolos de Comunicación Sistemas Embebidos

## Implementación de un driver generico basado en interrupciones para PCF8574

Driver genérico para el expansor de E/S PCF8574. Incluye portabilidad para STM32. Expone 
una API para el manejo del expansor similar a las utilizadas para el manejo de GPIOs. Permite
manejar las entradas y salidas del expansor en forma independiente.

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




