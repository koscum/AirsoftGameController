//
// Created by koscum on 29/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
#define AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H


#include <vector>

class I2cController
{
public:
	bool send(uint16_t _address, std::vector<uint8_t> *data);

	std::vector<uint8_t> *receive(uint16_t _address, uint16_t _size);

	static I2cController *getInstance();

private:
	I2cController();

	static I2cController *instance;
};


#endif //AIRSOFTGAMECONTROLLER_I2CCONTROLLER_H
