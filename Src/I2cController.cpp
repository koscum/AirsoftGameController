//
// Created by koscum on 29/06/2021.
//

#include <i2c.h>
#include "I2cController.h"
#include "stm32f4xx_hal_i2c.h"

bool I2cController::send(uint16_t _address, std::vector<uint8_t> *data)
{
	return HAL_I2C_Master_Transmit(
			&hi2c1,
			_address << 1,
			data->data(),
			data->size(),
			HAL_MAX_DELAY
	) == HAL_OK;
}

std::vector<uint8_t> *I2cController::receive(uint16_t _address, uint16_t _size)
{
	uint8_t buffer[_size];
	auto data = new std::vector<uint8_t>();

	if (HAL_I2C_Master_Receive(
			&hi2c1,
			_address << 1,
			buffer,
			_size,
			HAL_MAX_DELAY
	) == HAL_OK)
		for (uint16_t i = 0; i < _size; i++) data->push_back(buffer[i]);

	return data;
}

I2cController *I2cController::getInstance()
{
	if (!instance) instance = new I2cController();

	return instance;
}

I2cController::I2cController() = default;

I2cController *I2cController::instance = nullptr;
