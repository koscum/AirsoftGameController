#include <tim.h>
#include "MyMain.h"
#include "TimerManager.h"

void MyMain::main()
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);

	timer->start();

	const std::function<void()> *callback = new const std::function<void()>(
			[]() { HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); }
	);
	TimerManager::getInstance()->registerCallback(timer, callback);

	while (!exitCondition) {}

	TimerManager::getInstance()->unregisterCallback(timer, callback);
	delete callback;
}

void MyMain::extiCallback(uint16_t pin)
{
	exit();
}

void MyMain::timCallback(TIM_HandleTypeDef *handle)
{
	if (handle == &htim10) timer->tick();
}

MyMain *MyMain::getInstance()
{
	if (!instance) instance = new MyMain();

	return instance;
}

MyMain::MyMain()
{
	timer = new Timer(1000, true, true);
}

MyMain *MyMain::instance = nullptr;
