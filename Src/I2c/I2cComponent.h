//
// Created by koscum on 13/07/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_I2CCOMPONENT_H
#define AIRSOFTGAMECONTROLLER_I2CCOMPONENT_H


#include <cstdint>
#include <vector>

class I2cComponent
{
public:
	explicit I2cComponent(uint16_t address);

	bool transmit(std::vector<uint8_t> *data) const;

	bool writeRegister(uint16_t registerAddress, std::vector<uint8_t> *data) const;

	std::vector<uint8_t> *receive(uint16_t size) const;

	std::vector<uint8_t> *readRegister(uint16_t registerAddress, uint16_t size) const;

private:
	const uint16_t address;
};


#endif //AIRSOFTGAMECONTROLLER_I2CCOMPONENT_H
