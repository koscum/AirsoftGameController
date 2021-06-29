//
// Created by koscum on 16/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_TIMERMANAGER_H
#define AIRSOFTGAMECONTROLLER_TIMERMANAGER_H

#include <cstdint>
#include <functional>
#include <map>
#include <set>
#include "Timer.h"

class TimerManager
{
public:
	void registerCallback(const Timer *timer, const std::function<void()> *callback);

	void unregisterCallback(const Timer *timer, const std::function<void()> *callback);

	void registerTimer(const Timer *timer);

	void unregisterTimer(const Timer *timer);

	void finish(const Timer *timer);

	static TimerManager *getInstance();

private:
	TimerManager();

	std::map<const uintptr_t, std::set<const std::function<void()> *>> callbackDirectory;

	static TimerManager *instance;
};


#endif //AIRSOFTGAMECONTROLLER_TIMERMANAGER_H
