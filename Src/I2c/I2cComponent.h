#ifndef AIRSOFTGAMECONTROLLER_I2CCOMPONENT_H
#define AIRSOFTGAMECONTROLLER_I2CCOMPONENT_H


#include <atomic>
#include <cstdint>
#include <functional>
#include <vector>

class I2cComponent
{
public:
	I2cComponent() = delete;

	explicit I2cComponent(uint16_t address);

	I2cComponent(const I2cComponent &) = delete;

	auto operator=(const I2cComponent &) -> I2cComponent & = delete;

	auto busy() -> bool;

	auto ready() -> void;

	auto transmit(std::vector<uint8_t> *data,
	              const std::function<void()> *callback = nullptr) const -> void;

	auto writeRegister(uint16_t registerAddress,
	                   std::vector<uint8_t> *data,
	                   const std::function<void()> *callback = nullptr) const -> void;

	auto receive(uint16_t size,
	             const std::function<void(std::vector<uint8_t> *)> *callback = nullptr) const -> void;

	auto readRegister(uint16_t registerAddress,
	                  uint16_t size,
	                  const std::function<void(std::vector<uint8_t> *)> *callback = nullptr) const -> void;

private:
	const uint16_t address;

	std::atomic_bool isBusy;
};


#endif //AIRSOFTGAMECONTROLLER_I2CCOMPONENT_H
