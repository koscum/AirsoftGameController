#ifndef MIPS_TEST_MYMAIN_H
#define MIPS_TEST_MYMAIN_H

#include <stdint.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include "Timer.h"

class MyMain
{
public:
	void main();

	void exit() { exitCondition = true; };

	void extiCallback(uint16_t pin);

	void timCallback(TIM_HandleTypeDef *handle);

	static MyMain *getInstance();

private:
	MyMain();

	bool exitCondition = false;
	Timer *timer;

	static MyMain *instance;
};

#endif //MIPS_TEST_MYMAIN_H
