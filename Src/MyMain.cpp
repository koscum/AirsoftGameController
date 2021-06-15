#include <tim.h>
#include "MyMain.h"

void MyMain::main()
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);

	timer->start();

	while (true)
	{
		if (timer->getValue() >= 1000)
		{
			timer->reset();
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			HAL_Delay(100);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}
	}
}

void MyMain::extiCallback(uint16_t pin)
{
	if (timer->isRunning()) timer->stop();
	else timer->start();
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
	// TODO: Init instance.
	timer = new Timer();
}

MyMain *MyMain::instance = 0L;
