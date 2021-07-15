#include <tim.h>
#include "Hardware/Keypad4x4.h"
#include "Hardware/Matrix8x8.h"
#include "Hardware/SevenSegment.h"
#include "MyMain.h"
#include "TimerManager.h"

void MyMain::init()
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);
}

void MyMain::main()
{
	auto timer1 = Timer(1000, true, true);
	auto blueSegment = SevenSegment(0x74);
	uint64_t time = 0;

	const auto blueSegmentUpdate = [&blueSegment, &time]()
	{
		blueSegment.setDigit(0, (time / 600) % 6);
		blueSegment.setDigit(1, (time / 60) % 10);
		blueSegment.setDigit(2, (time / 10) % 6);
		blueSegment.setDigit(3, time % 10);
	};

	const std::function<void()> callback1 = std::function<void()>(
			[&blueSegment, &blueSegmentUpdate, &time]()
			{
				++time;
				blueSegmentUpdate();
				blueSegment.toggleColon();
				blueSegment.writeDisplay();
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			}
	);

	blueSegment.begin();
	blueSegment.setBlinkRate(LedBackpack::BlinkRate::OFF);
	blueSegment.setBrightness(3);
	blueSegmentUpdate();
	blueSegment.writeDisplay();

	TimerManager::getInstance()->registerCallback(&timer1, &callback1);

	timer1.start();

	while (!exitCondition) HAL_Delay(1); // Do nothing

	TimerManager::getInstance()->unregisterCallback(&timer1, &callback1);
}

void MyMain::extiCallback(uint16_t pin)
{
	exit();
}

void MyMain::timCallback(const TIM_HandleTypeDef *handle)
{
	if (handle == &htim10) TimerManager::getInstance()->tick();
}

MyMain *MyMain::getInstance()
{
	if (!instance) instance = new MyMain();

	return instance;
}

MyMain::MyMain() = default;

MyMain *MyMain::instance = nullptr;
