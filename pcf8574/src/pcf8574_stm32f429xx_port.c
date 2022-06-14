/*
 * pcf8574_stm32f429xx_port.c
 *
 *  Created on: Jun 5, 2022
 *      Author: gianfranco
 */

#include "pcf8574_stm32f429xx_port.h"
#include "pcf8574.h"
#include "pcf8574_cfg.h"
#include "stm32f4xx_hal.h"

#include <string.h>

#define I2C_ADDRESS (((uint32_t) PCF8574_ADDRESS) << 1)


I2C_HandleTypeDef i2c_handle = {0};


void PCF8574_PORT_init_pins(void) {
	/* Initializes I2C pins. */
	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitTypeDef gpio_i2c = {
			.Pin = GPIO_PIN_0 | GPIO_PIN_1,
			.Mode = GPIO_MODE_AF_OD,
			.Pull = GPIO_PULLUP,
			.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
			.Alternate = GPIO_AF4_I2C2
	};

	HAL_GPIO_Init(GPIOF, &gpio_i2c);

	/* Initializes INT pin. */
	GPIO_InitTypeDef gpio_int = {
			.Pin = GPIO_PIN_2,
			.Mode = GPIO_MODE_IT_FALLING,
			.Pull = GPIO_PULLUP
	};

	HAL_GPIO_Init(GPIOF, &gpio_int);
}

pcf8574_status PCF8574_PORT_init_i2c(void) {
	/* I2C clock enable */
	__HAL_RCC_I2C2_CLK_ENABLE();

	/* I2C configuration */
	i2c_handle.Instance = I2C2;
	i2c_handle.Init.ClockSpeed = I2C_SPEED_HZ;
	i2c_handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	i2c_handle.Init.OwnAddress1 = 0;
	i2c_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c_handle.Init.OwnAddress2 = 0;
	i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c_handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	/* Initializing I2C */
	HAL_StatusTypeDef status = HAL_I2C_Init(&i2c_handle);

	if (status != HAL_OK) {
		return PCF8574_ERROR;
	}

	/* Configure analog filter */
	status = HAL_I2CEx_ConfigAnalogFilter(&i2c_handle, I2C_ANALOGFILTER_ENABLE);

	if (status != HAL_OK) {
		return PCF8574_ERROR;
	}

	/* Configure digital filter */
	status = HAL_I2CEx_ConfigDigitalFilter(&i2c_handle, 0);

	if (status != HAL_OK) {
		return PCF8574_ERROR;
	}

	return PCF8574_OK;
}

void PCF8574_PORT_enable_interrupts(void) {
	/* Enabling i2c related interrupts */
	HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
	/* Enabling GPIO interrupt */
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

pcf8574_status PCF8574_PORT_write_register(uint8_t *data) {
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_IT(&i2c_handle, I2C_ADDRESS, data, 1);

	if (status != HAL_OK) {
		return PCF8574_ERROR;
	}

	return PCF8574_OK;
}

void PCF8574_PORT_read_register(uint8_t *data) {
	HAL_I2C_Master_Receive_IT(&i2c_handle, I2C_ADDRESS, data, 1);
}

void I2C2_EV_IRQHandler(void) {
	HAL_I2C_EV_IRQHandler(&i2c_handle);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	_PCF8574_rx_transfer_completed_callback();
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	_PCF8574_tx_transfer_completed_callback();
}

void EXTI2_IRQHandler(void) {
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	_PCF8574_interrupt_callback();
}
