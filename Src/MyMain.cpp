#include <tim.h>
#include <i2c.h>
#include <random>
#include "Hardware/Keypad4x4.h"
#include "Hardware/Matrix8x8.h"
#include "Hardware/SevenSegment.h"
#include "I2c/I2cController.h"
#include "MyMain.h"
#include "TimerManager.h"

auto MyMain::init() -> void
{
	HAL_TIM_Base_Start_IT(&htim10); // Start TIM10 under Interrupt
	HAL_TIM_OC_Start(&htim10, TIM_CHANNEL_1);
}

auto MyMain::main() -> void
{
	auto i2cController = I2cController::getInstance();
	auto timerManager = TimerManager::getInstance();

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

	const auto callback1 = std::function<void()>(
			[&]()
			{
				++time;
				blueSegmentUpdate();
			}
	);

	const auto keypadCallback = std::function<void()>(
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

	timerManager->registerCallback(&timer1,
	                               &callback1);
	timerManager->registerCallback(&displayRefreshTimer,
	                               &displayRefreshCallback);
	timerManager->registerCallback(&yellowSegmentRandomTimer,
	                               &yellowSegmentRandomCallback);
	timerManager->registerCallback(&keypadTimer,
	                               &keypadCallback);

	timer1.start();
	yellowSegmentRandomTimer.start();
	displayRefreshTimer.start();
	keypadTimer.start();

	while (!exitCondition)
	{
		if (doTimerTick.exchange(false))
		{
			timerManager->tick();
		}
		if (doI2cRequestCompleted.exchange(false))
		{
			i2cController->requestCompleted(i2cError.exchange(0U));
		}

		i2cController->tick();
	}

	keypadTimer.stop();
	displayRefreshTimer.stop();
	yellowSegmentRandomTimer.stop();
	timer1.stop();

	timerManager->unregisterCallback(&keypadTimer,
	                                 &keypadCallback);
	timerManager->unregisterCallback(&displayRefreshTimer,
	                                 &displayRefreshCallback);
	timerManager->unregisterCallback(&yellowSegmentRandomTimer,
	                                 &yellowSegmentRandomCallback);
	timerManager->unregisterCallback(&timer1,
	                                 &callback1);
}

auto MyMain::externalInterruptCallback(uint16_t pin) -> void
{
	exit();
}

auto MyMain::timCallback(const TIM_HandleTypeDef *handle) -> void
{
	if (handle == &htim10) doTimerTick = true;
}

auto MyMain::i2cCompletedCallback(const I2C_HandleTypeDef *handle, uint8_t error) -> void
{
	if (handle == &hi2c1)
	{
		doI2cRequestCompleted = true;
		i2cError = error;
	}
}

auto MyMain::getInstance() -> MyMain *
{
	if (!instance) instance = new MyMain();

	return instance;
}

MyMain::MyMain() = default;

MyMain *MyMain::instance = nullptr;
