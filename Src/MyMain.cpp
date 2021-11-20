#include <tim.h>
#include <i2c.h>
#include <random>
#include "Hardware/Keypad4x4.h"
#include "Hardware/Matrix8x8.h"
#include "Hardware/SevenSegment.h"
#include "I2c/I2cController.h"
#include "MyMain.h"
#include "TimerManager.h"

void MyMain::init()
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);
}

void MyMain::main()
{
	auto i2cController = I2cController::getInstance();

	auto timer1 = Timer(500, true, true);
	auto displayRefreshTimer = Timer(20, true, true);
	auto yellowSegmentRandomTimer = Timer(250, true, true);
	auto keypadTimer = Timer(10, true, true);

	auto blueSegment = SevenSegment(0x74);
	auto yellowSegment = SevenSegment(0x76);
	auto keypad = Keypad4x4(0x24);

	uint64_t time = 0;

	auto randomDevice = std::random_device();

	auto randomDigitPosition =
			[&, distribution = std::uniform_int_distribution<uint8_t>(0, 3)]() mutable -> uint8_t
			{
				return distribution(randomDevice);
			};

	auto randomDigitValue =
			[&, distribution = std::uniform_int_distribution<uint8_t>(0, 15)]() mutable -> uint8_t
			{
				return distribution(randomDevice);
			};

	const auto blueSegmentUpdate = [&]()
	{
		blueSegment.setDigit(0, (time / 600 / 2) % 6);
		blueSegment.setDigit(1, (time / 60 / 2) % 10);
		blueSegment.setDigit(2, (time / 10 / 2) % 6);
		blueSegment.setDigit(3, time / 2 % 10);
		blueSegment.setColon(time % 2 < 1);
	};

	const auto yellowSegmentUpdate = [&]()
	{
		yellowSegment.setDigit(randomDigitPosition(),
		                       randomDigitValue());
		yellowSegment.toggleColon();
	};

	const auto displayRefreshCallback = std::function<void()>(
			[&]()
			{
				blueSegment.writeDisplay();
				yellowSegment.writeDisplay();
			}
	);

	const auto yellowSegmentRandomCallback = std::function<void()>(
			[&]()
			{
				yellowSegmentUpdate();
			}
	);

	const std::function<void()> callback1 = std::function<void()>(
			[&]()
			{
				++time;
				blueSegmentUpdate();
			}
	);

	const std::function<void()> keypadCallback = std::function<void()>(
			[&]()
			{
				keypad.tick();
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

	keypad.init();

	TimerManager::getInstance()->registerCallback(&timer1,
	                                              &callback1);
	TimerManager::getInstance()->registerCallback(&displayRefreshTimer,
	                                              &displayRefreshCallback);
	TimerManager::getInstance()->registerCallback(&yellowSegmentRandomTimer,
	                                              &yellowSegmentRandomCallback);
	TimerManager::getInstance()->registerCallback(&keypadTimer,
	                                              &keypadCallback);

	timer1.start();
	yellowSegmentRandomTimer.start();
	displayRefreshTimer.start();
	keypadTimer.start();

	while (!exitCondition)
	{
		if (doTimerTick.exchange(false))
		{
			TimerManager::getInstance()->tick();
		}
		if (doI2cRequestCompleted.exchange(false))
		{
			I2cController::getInstance()->requestCompleted();
		}

		i2cController->tick();
	}

	keypadTimer.stop();
	displayRefreshTimer.stop();
	yellowSegmentRandomTimer.stop();
	timer1.stop();

	TimerManager::getInstance()->unregisterCallback(&keypadTimer,
	                                                &keypadCallback);
	TimerManager::getInstance()->unregisterCallback(&displayRefreshTimer,
	                                                &displayRefreshCallback);
	TimerManager::getInstance()->unregisterCallback(&yellowSegmentRandomTimer,
	                                                &yellowSegmentRandomCallback);
	TimerManager::getInstance()->unregisterCallback(&timer1,
	                                                &callback1);
}

void MyMain::extiCallback(uint16_t pin)
{
	exit();
}

void MyMain::timCallback(const TIM_HandleTypeDef *handle)
{
	if (handle == &htim10) doTimerTick = true;
}

void MyMain::i2cCpltCallback(const I2C_HandleTypeDef *handle)
{
	if (handle == &hi2c1) doI2cRequestCompleted = true;
}

MyMain *MyMain::getInstance()
{
	if (!instance) instance = new MyMain();

	return instance;
}

MyMain::MyMain() = default;

MyMain *MyMain::instance = nullptr;
