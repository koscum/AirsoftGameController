//
// Created by koscum on 14/06/2021.
//

#include "Timer.h"
#include "TimerManager.h"

Timer::Timer(const uint32_t targetParam, const bool countUpParam, const bool repeatParam) :
		target(targetParam),
		countUp(countUpParam),
		repeat(repeatParam) {}

Timer::~Timer() = default;

void Timer::tick()
{
	if (running)
	{
		advanceTimer();
		if (target != 0 && (countUp && value >= target || !countUp && value <= 0)) finish();
	}
}

void Timer::start()
{
	if (!running && (target != 0 || countUp))
	{
		previousTimestamp = HAL_GetTick();
		running = true;
	}
}

void Timer::stop()
{
	if (running)
	{
		advanceTimer();
		running = false;
	}
}

void Timer::reset()
{
	previousTimestamp = HAL_GetTick();
	if (countUp) value = 0;
	else value = target;
}

void Timer::setValue(int64_t valueParam)
{
	value = valueParam;
	if (target != 0 && (countUp && value >= target || !countUp && value <= 0)) finish();
}

void Timer::advanceTimer()
{
	uint32_t timestamp = HAL_GetTick();
	uint32_t elapsed = timestamp - previousTimestamp;

	previousTimestamp = timestamp;

	if (countUp) value += elapsed;
	else value -= elapsed;
}

void Timer::finish()
{
	if (!repeat) stop();

	TimerManager::getInstance()->finish(this);

	if (repeat) reset();
}
