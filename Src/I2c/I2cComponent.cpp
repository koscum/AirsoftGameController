#include "I2cComponent.h"
#include "I2cController.h"

I2cComponent::I2cComponent(uint16_t _address) :
		address(_address),
		isBusy(false) {}

auto I2cComponent::busy() -> bool
{
	return isBusy.exchange(true);
}

auto I2cComponent::ready() -> void
{
	isBusy = false;
}

auto I2cComponent::transmit(std::vector<uint8_t> *_data,
                            const std::function<void()> *_callback) const -> void
{
	I2cController::getInstance()->request(new I2cController::I2cRequestTx(address,
	                                                                      _data,
	                                                                      _callback));
}

auto I2cComponent::writeRegister(uint16_t _registerAddress,
                                 std::vector<uint8_t> *_data,
                                 const std::function<void()> *_callback) const -> void
{
	I2cController::getInstance()->request(new I2cController::I2cRequestTx(address,
	                                                                      _registerAddress,
	                                                                      _data,
	                                                                      _callback));
}

auto I2cComponent::receive(uint16_t _size,
                           const std::function<void(std::vector<uint8_t> *)> *_callback) const -> void
{
	I2cController::getInstance()->request(new I2cController::I2cRequestRx(address,
	                                                                      _size,
	                                                                      _callback));
}

auto I2cComponent::readRegister(uint16_t _registerAddress,
                                uint16_t _size,
                                const std::function<void(std::vector<uint8_t> *)> *_callback) const -> void
{
	I2cController::getInstance()->request(new I2cController::I2cRequestRx(address,
	                                                                      _registerAddress,
	                                                                      _size,
	                                                                      _callback));
}
