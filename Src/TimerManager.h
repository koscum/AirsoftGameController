//
// Created by koscum on 16/06/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_TIMERMANAGER_H
#define AIRSOFTGAMECONTROLLER_TIMERMANAGER_H

#include <stdint.h>
#include <functional>
#include <list>
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
	struct Entry
	{
		Entry(const Timer *timer) : timer(timer) {};

		const Timer *timer;
		std::list<const std::function<void()> *> callbacks;
	};

	TimerManager();

	std::list<Entry *> entries;

	static TimerManager *instance;
};


#endif //AIRSOFTGAMECONTROLLER_TIMERMANAGER_H
