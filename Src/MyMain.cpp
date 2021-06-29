#include <tim.h>
#include "MyMain.h"
#include "TimerManager.h"

void MyMain::init()
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);
}

void MyMain::main()
{
	auto *timer = new Timer(1000, true, true);
	const std::function<void()> *callback = new const std::function<void()>(
			[]() { HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); }
	);

	TimerManager::getInstance()->registerTimer(&htim10, timer);
	TimerManager::getInstance()->registerCallback(timer, callback);

	timer->start();

	while (!exitCondition) {}

	TimerManager::getInstance()->unregisterCallback(timer, callback);
	TimerManager::getInstance()->unregisterTimer(&htim10, timer);

	delete callback;
}

void MyMain::extiCallback(uint16_t pin)
{
	exit();
}

void MyMain::timCallback(const TIM_HandleTypeDef *handle)
{
	TimerManager::getInstance()->tick(handle);
}

MyMain *MyMain::getInstance()
{
	if (!instance) instance = new MyMain();

	return instance;
}

MyMain::MyMain() = default;

MyMain *MyMain::instance = nullptr;
