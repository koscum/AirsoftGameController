#ifndef AIRSOFTGAMECONTROLLER_MYMAIN_H
#define AIRSOFTGAMECONTROLLER_MYMAIN_H


#include <atomic>
#include <cstdint>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>

class MyMain
{
public:
	MyMain(const MyMain &) = delete;

	auto operator=(const MyMain &) -> MyMain & = delete;

	auto init() -> void;

	auto main() -> void;

	auto exit() -> void { exitCondition = true; };

	auto externalInterruptCallback(uint16_t pin) -> void;

	auto timCallback(const TIM_HandleTypeDef *handle) -> void;

	auto i2cCompletedCallback(const I2C_HandleTypeDef *handle) -> void;

	static auto getInstance() -> MyMain *;

private:
	MyMain();

	std::atomic_bool doTimerTick = false;
	std::atomic_bool doI2cRequestCompleted = false;

	bool exitCondition = false;

	static MyMain *instance;
};


#endif //AIRSOFTGAMECONTROLLER_MYMAIN_H
