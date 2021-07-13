//
// Created by koscum on 13/07/2021.
//

#include "I2cComponent.h"
#include "I2cController.h"

I2cComponent::I2cComponent(uint16_t _address) :
		address(_address) {}

bool I2cComponent::transmit(std::vector<uint8_t> *_data) const
{
	return I2cController::getInstance()->transmit(address, _data);
}

bool I2cComponent::writeRegister(uint16_t _registerAddress, std::vector<uint8_t> *_data) const
{
	return I2cController::getInstance()->writeRegister(address, _registerAddress, _data);
}

std::vector<uint8_t> *I2cComponent::receive(uint16_t _size) const
{
	return I2cController::getInstance()->receive(address, _size);
}

std::vector<uint8_t> *I2cComponent::readRegister(uint16_t _registerAddress, uint16_t _size) const
{
	return I2cController::getInstance()->readRegister(address, _registerAddress, _size);
}
