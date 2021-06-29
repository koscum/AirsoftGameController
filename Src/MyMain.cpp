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
	auto *timer1 = new Timer(1000, true, true);
	auto *timer2 = new Timer(500, true, true);
	const std::function<void()> *callback1 = new const std::function<void()>(
			[timer2]()
			{
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				timer2->start();
			}
	);
	const std::function<void()> *callback2 = new const std::function<void()>(
			[timer2]()
			{
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				timer2->stop();
			}
	);

	TimerManager::getInstance()->registerTimer(&htim10, timer1);
	TimerManager::getInstance()->registerTimer(&htim10, timer2);
	TimerManager::getInstance()->registerCallback(timer1, callback1);
	TimerManager::getInstance()->registerCallback(timer2, callback2);

	timer1->start();

	while (!exitCondition) {}

	TimerManager::getInstance()->unregisterCallback(timer1, callback1);
	TimerManager::getInstance()->unregisterCallback(timer2, callback2);
	TimerManager::getInstance()->unregisterTimer(&htim10, timer1);
	TimerManager::getInstance()->unregisterTimer(&htim10, timer2);

	delete callback1;
	delete callback2;
	delete timer1;
	delete timer2;
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
