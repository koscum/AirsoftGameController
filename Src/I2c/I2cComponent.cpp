//
// Created by koscum on 13/07/2021.
//

#include "I2cComponent.h"
#include "I2cController.h"

I2cComponent::I2cComponent(uint16_t _address) :
		address(_address),
		isBusy(false) {}

bool I2cComponent::busy()
{
	isBusy.exchange(true);
}

void I2cComponent::ready()
{
	isBusy = false;
}

void I2cComponent::transmit(std::vector<uint8_t> *_data,
                            const std::function<void()> *_callback) const
{
	I2cController::getInstance()->request(new I2cController::I2cRequestTx(address,
	                                                                      _data,
	                                                                      _callback));
}

void I2cComponent::writeRegister(uint16_t _registerAddress,
                                 std::vector<uint8_t> *_data,
                                 const std::function<void()> *_callback) const
{
	I2cController::getInstance()->request(new I2cController::I2cRequestTx(address,
	                                                                      _registerAddress,
	                                                                      _data,
	                                                                      _callback));
}

void I2cComponent::receive(uint16_t _size,
                           const std::function<void(std::vector<uint8_t> *)> *_callback) const
{
	I2cController::getInstance()->request(new I2cController::I2cRequestRx(address,
	                                                                      _size,
	                                                                      _callback));
}

void I2cComponent::readRegister(uint16_t _registerAddress,
                                uint16_t _size,
                                const std::function<void(std::vector<uint8_t> *)> *_callback) const
{
	I2cController::getInstance()->request(new I2cController::I2cRequestRx(address,
	                                                                      _registerAddress,
	                                                                      _size,
	                                                                      _callback));
}
