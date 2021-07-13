//
// Created by koscum on 13/07/2021.
//

#include "I2cComponent.h"
#include "I2cController.h"

I2cComponent::I2cComponent(uint16_t _address) :
		address(_address) {}

bool I2cComponent::send(std::vector<uint8_t> *data) const
{
	return I2cController::getInstance()->send(address, data);
}

std::vector<uint8_t> *I2cComponent::receive(uint16_t _size) const
{
	return I2cController::getInstance()->receive(address, _size);
}
