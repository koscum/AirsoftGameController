//
// Created by koscum on 13/07/2021.
//

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

	I2cComponent &operator=(const I2cComponent &) = delete;

	bool busy();

	void ready();

	void transmit(std::vector<uint8_t> *data,
	              const std::function<void()> *callback = nullptr) const;

	void writeRegister(uint16_t registerAddress,
	                   std::vector<uint8_t> *data,
	                   const std::function<void()> *callback = nullptr) const;

	void receive(uint16_t size,
	             const std::function<void(std::vector<uint8_t> *)> *callback = nullptr) const;

	void readRegister(uint16_t registerAddress,
	                  uint16_t size,
	                  const std::function<void(std::vector<uint8_t> *)> *callback = nullptr) const;

private:
	const uint16_t address;

	std::atomic_bool isBusy;
};


#endif //AIRSOFTGAMECONTROLLER_I2CCOMPONENT_H
