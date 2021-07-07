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
	auto timer3 = Timer(125, true, true);
	auto blueSegment = SevenSegment(0x74);
	auto yellowSegment = SevenSegment(0x76);
	uint64_t time = 0;

	uint8_t dotToggle = 0;

	const auto blueSegmentUpdate = [&blueSegment, &time]()
	{
		blueSegment.setDigit(0, (time / 600) % 6);
		blueSegment.setDigit(1, (time / 60) % 10);
		blueSegment.setDigit(2, (time / 10) % 6);
		blueSegment.setDigit(3, time % 10);
	};

	const auto yellowSegmentUpdate = [&yellowSegment, &time]()
	{
		yellowSegment.setDigit(3, (time / 3600) % 24 % 10);
		yellowSegment.setDigit(2, (time / 3600) % 24 / 10);
	};

	const std::function<void()> callback1 = std::function<void()>(
			[&blueSegment, &blueSegmentUpdate, &time, &timer2, &yellowSegment, &yellowSegmentUpdate]()
			{
				++time;
				blueSegmentUpdate();
				blueSegment.toggleColon();
				blueSegment.writeDisplay();
				yellowSegmentUpdate();
				yellowSegment.writeDisplay();
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

	const std::function<void()> callback3 = std::function<void()>(
			[&blueSegment, &dotToggle, &yellowSegment]()
			{
				if (dotToggle > 3)
				{
					blueSegment.toggleDot(dotToggle % 4);
					blueSegment.writeDisplay();
				} else if (dotToggle < 4)
				{
					yellowSegment.toggleDot(dotToggle % 4);
					yellowSegment.writeDisplay();
				}

				++dotToggle;
				dotToggle %= 8;
			}
	);

	blueSegment.begin();
	blueSegment.setBlinkRate(LedBackpack::BlinkRate::OFF);
	blueSegment.setBrightness(3);
	blueSegmentUpdate();
	blueSegment.writeDisplay();

	yellowSegment.begin();
	yellowSegment.setBlinkRate(LedBackpack::BlinkRate::OFF);
	yellowSegment.setBrightness(3);
	yellowSegmentUpdate();
	yellowSegment.writeDisplay();

	TimerManager::getInstance()->registerCallback(&timer1, &callback1);
	TimerManager::getInstance()->registerCallback(&timer2, &callback2);
	TimerManager::getInstance()->registerCallback(&timer3, &callback3);

	timer1.start();
	timer3.start();

	while (!exitCondition) {}

	TimerManager::getInstance()->unregisterCallback(&timer1, &callback1);
	TimerManager::getInstance()->unregisterCallback(&timer2, &callback2);
	TimerManager::getInstance()->unregisterCallback(&timer3, &callback3);
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
