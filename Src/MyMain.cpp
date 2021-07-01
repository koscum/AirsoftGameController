#include <tim.h>
#include "MyMain.h"
#include "TimerManager.h"
#include "SevenSegment.h"

void MyMain::init()
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);
}

void MyMain::main()
{
	auto timer1 = Timer(1000, true, true);
	auto timer2 = Timer(500, true, true);
	auto blueSegment = SevenSegment(0x74);
	uint64_t time = 0;

	const auto blueSegmentUpdate = [&blueSegment, &time]()
	{
		blueSegment.setDigit(0, (time / 600) % 6);
		blueSegment.setDigit(1, (time / 60) % 10);
		blueSegment.setDigit(2, (time / 10) % 6);
		blueSegment.setDigit(3, time % 10);

		blueSegment.setDot(0, (time / 3600) % 12 & 0x08);
		blueSegment.setDot(1, (time / 3600) % 12 & 0x04);
		blueSegment.setDot(2, (time / 3600) % 12 & 0x02);
		blueSegment.setDot(3, (time / 3600) % 12 & 0x01);
	};

	const std::function<void()> callback1 = std::function<void()>(
			[&blueSegment, &blueSegmentUpdate, &time, &timer2]()
			{
				++time;
				blueSegmentUpdate();
				blueSegment.toggleColon();
				blueSegment.writeDisplay();
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				timer2.start();
			}
	);

	const std::function<void()> callback2 = std::function<void()>(
			[&blueSegment, &timer2]()
			{
				blueSegment.toggleColon();
				blueSegment.writeColon();
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				timer2.stop();
			}
	);

	blueSegment.begin();
	blueSegment.setBlinkRate(LedBackpack::BlinkRate::OFF);
	blueSegment.setBrightness(3);
	blueSegmentUpdate();

	TimerManager::getInstance()->registerCallback(&timer1, &callback1);
	TimerManager::getInstance()->registerCallback(&timer2, &callback2);

	timer1.start();

	while (!exitCondition) {}

	TimerManager::getInstance()->unregisterCallback(&timer1, &callback1);
	TimerManager::getInstance()->unregisterCallback(&timer2, &callback2);
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
