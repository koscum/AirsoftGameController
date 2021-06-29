//
// Created by koscum on 14/06/2021.
//

#include <limits>
#include "Timer.h"
#include "TimerManager.h"

Timer::Timer(const uint32_t _target, const bool _countUp, const bool _repeat) :
		target(_target),
		countUp(_target == 0 || _countUp),
		repeat(_target != 0 && _repeat) {}

Timer::~Timer() = default;

void Timer::tick()
{
	if (running) advanceTimer();
}

void Timer::start()
{
	if (!running)
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

void Timer::setValue(int64_t _value)
{
	value = _value;
	if (target != 0 && countUp ? value >= target : value <= 0) finish();
}

void Timer::advanceTimer()
{
	uint32_t timestamp = HAL_GetTick();
	uint32_t elapsed =
			timestamp >= previousTimestamp ?
			timestamp - previousTimestamp :
			static_cast<uint32_t>(
					(static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) + timestamp - previousTimestamp)
					% std::numeric_limits<uint32_t>::max()
			);

	previousTimestamp = timestamp;

	setValue(countUp ? value + elapsed : value - elapsed);
}

void Timer::finish()
{
	if (repeat) reset();
	else stop();

	TimerManager::getInstance()->finish(this);
}
