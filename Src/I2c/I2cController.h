//
// Created by koscum on 29/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
#define AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H


#include <vector>

class I2cController
{
public:
	bool transmit(uint16_t address, std::vector<uint8_t> *data);

	bool writeRegister(uint16_t address, uint16_t registerAddress, std::vector<uint8_t> *data);

	std::vector<uint8_t> *receive(uint16_t address, uint16_t size);

	std::vector<uint8_t> *readRegister(uint16_t address, uint16_t registerAddress, uint16_t size);

	static I2cController *getInstance();

private:
	I2cController();

	static I2cController *instance;
};


#endif //AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
