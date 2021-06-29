//
// Created by koscum on 14/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_TIMER_H
#define AIRSOFTGAMECONTROLLER_TIMER_H


#include <cstdint>

class Timer
{
public:
	explicit Timer(uint32_t _target = 0, bool _countUp = true, bool _repeat = false);

	~Timer();

	void tick();

	void start();

	void stop();

	void reset();

	int64_t getValue() const { return value; }

	bool isRunning() const { return running; }

protected:
	void setValue(int64_t _value);

private:
	void advanceTimer();

	void finish();

	const uint32_t target;
	const bool countUp;
	const bool repeat;
	bool running = false;
	int64_t value = 0;
	uint32_t previousTimestamp = 0;
};


#endif //AIRSOFTGAMECONTROLLER_TIMER_H
