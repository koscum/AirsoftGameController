//
// Created by koscum on 14/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_TIMER_H
#define AIRSOFTGAMECONTROLLER_TIMER_H

#include <stdint.h>
#include <stm32f4xx_hal.h>

class Timer
{
public:
	Timer(const uint32_t target = 0, const bool countUp = true);

	void tick();

	void start();
	void stop();
	void reset();

	int64_t getValue() { return value; }
	bool isRunning() { return running; }

protected:
	void setValue(int64_t value);

private:
	void advanceTimer();
	void finish();

	const uint32_t target;
	const bool countUp = true;
	int64_t value = 0;
	bool running = false;
	uint32_t previousTick = 0;
};


#endif //AIRSOFTGAMECONTROLLER_TIMER_H
