#include <tim.h>
#include "MyMain.h"

void MyMain::main()
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);

	while (true)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(500);
	}
}

void MyMain::extiCallback(uint16_t pin)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void MyMain::timCallback(TIM_HandleTypeDef *handle)
{
	if (handle == &htim10)
	{
		// Handle TIM10 interrupt
	}
}

MyMain *MyMain::getInstance()
{
	if (!instance)
	{
		instance = new MyMain();
	}

	return instance;
}

MyMain::MyMain()
{
	// TODO: Init instance.
}

MyMain *MyMain::instance = 0L;
