#ifndef AIRSOFTGAMECONTROLLER_TIMER_H
#define AIRSOFTGAMECONTROLLER_TIMER_H


#include <cstdint>

class Timer
{
public:
	Timer() = delete;

	explicit Timer(uint32_t _target = 0, bool _countUp = true, bool _repeat = false);

	Timer(const Timer &) = delete;

	auto operator=(const Timer &) -> Timer & = delete;

	~Timer();

	auto tick() -> void;

	auto start() -> void;

	auto stop() -> void;

	auto reset() -> void;

	[[nodiscard]] auto getValue() const -> int64_t { return value; }

	[[nodiscard]] auto isRunning() const -> bool { return running; }

protected:
	auto setValue(int64_t _value) -> void;

private:
	auto advanceTimer() -> void;

	auto finish() -> void;

	const uint32_t target;
	const bool countUp;
	const bool repeat;
	bool running = false;
	int64_t value = 0;
	uint32_t previousTimestamp = 0;
};


#endif //AIRSOFTGAMECONTROLLER_TIMER_H
