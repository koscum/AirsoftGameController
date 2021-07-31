//
// Created by koscum on 16/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_TIMERMANAGER_H
#define AIRSOFTGAMECONTROLLER_TIMERMANAGER_H


#include <functional>
#include <map>
#include <set>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include "Timer.h"

class TimerManager
{
public:
	TimerManager(const TimerManager &) = delete;

	TimerManager &operator=(const TimerManager &) = delete;

	void registerCallback(Timer *timer, const std::function<void()> *callback);

	void unregisterCallback(Timer *timer, const std::function<void()> *callback);

	void registerTimer(Timer *timer);

	void unregisterTimer(Timer *timer);

	void finish(Timer *timer);

	void tick();

	static TimerManager *getInstance();

private:
	TimerManager();

	std::map<Timer *, std::set<const std::function<void()> *>> callbackDirectory;

	static TimerManager *instance;
};


#endif //AIRSOFTGAMECONTROLLER_TIMERMANAGER_H
