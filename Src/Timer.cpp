//
// Created by koscum on 14/06/2021.
//

#include "Timer.h"

Timer::Timer(const uint32_t targetParam, const bool countUpParam) :
		target(targetParam),
		countUp(countUpParam)
{
	// TODO: Register timer.
}

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
		previousTick = HAL_GetTick();
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
	previousTick = HAL_GetTick();
	value = target;
}

void Timer::setValue(int64_t valueParam)
{
	value = valueParam;
	if (target != 0 && (countUp && value >= target || !countUp && value <= 0)) finish();
}

void Timer::advanceTimer()
{
	uint32_t tick = HAL_GetTick();
	uint32_t elapsed = tick - previousTick;

	previousTick = tick;

	if (countUp) value += elapsed;
	else value -= elapsed;
}

void Timer::finish()
{
	stop();

	// TODO: Signal completion.
}
