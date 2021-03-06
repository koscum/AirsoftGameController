#ifndef AIRSOFTGAMECONTROLLER_MYMAIN_H
#define AIRSOFTGAMECONTROLLER_MYMAIN_H


#include <cstdint>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>

class MyMain
{
public:
	MyMain(const MyMain &) = delete;

	MyMain &operator=(const MyMain &) = delete;

	void init();

	void main();

	void exit() { exitCondition = true; };

	void extiCallback(uint16_t pin);

	void timCallback(const TIM_HandleTypeDef *handle);

	void i2cCpltCallback(const I2C_HandleTypeDef *handle);

	static MyMain *getInstance();

private:
	MyMain();

	bool exitCondition = false;

	static MyMain *instance;
};


#endif //AIRSOFTGAMECONTROLLER_MYMAIN_H
