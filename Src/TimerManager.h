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

	auto operator=(const TimerManager &) -> TimerManager & = delete;

	auto registerCallback(Timer *timer, const std::function<void()> *callback) -> void;

	auto unregisterCallback(Timer *timer, const std::function<void()> *callback) -> void;

	auto registerTimer(Timer *timer) -> void;

	auto unregisterTimer(Timer *timer) -> void;

	auto finish(Timer *timer) -> void;

	auto tick() -> void;


	static auto getInstance() -> TimerManager *;

private:
	TimerManager();

	std::map<Timer *, std::set<const std::function<void()> *>> callbackDirectory;

	static TimerManager *instance;
};


#endif //AIRSOFTGAMECONTROLLER_TIMERMANAGER_H
