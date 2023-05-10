#include <limits>
#include "Timer.h"
#include "TimerManager.h"

Timer::Timer(const uint32_t _target, const bool _countUp, const bool _repeat) :
		target(_target),
		countUp(_target == 0 || _countUp),
		repeat(_target != 0 && _repeat)
{
	TimerManager::getInstance()->registerTimer(this);
}

Timer::~Timer()
{
	TimerManager::getInstance()->unregisterTimer(this);
}

auto Timer::tick() -> void
{
	if (running) advanceTimer();
}

auto Timer::start() -> void
{
	if (!running)
	{
		previousTimestamp = HAL_GetTick();
		running = true;
	}
}

auto Timer::stop() -> void
{
	if (running)
	{
		advanceTimer();
		running = false;
	}
}

auto Timer::reset() -> void
{
	previousTimestamp = HAL_GetTick();
	if (countUp)
	{ value = 0; }
	else
	{ value = target; }
}

auto Timer::setValue(int64_t _value) -> void
{
	value = _value;
	if (target != 0 && countUp ? value >= target : value <= 0) finish();
}

auto Timer::advanceTimer() -> void
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

auto Timer::finish() -> void
{
	if (repeat)
	{ reset(); }
	else
	{ stop(); }

	TimerManager::getInstance()->finish(this);
}
