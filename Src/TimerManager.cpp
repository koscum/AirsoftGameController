//
// Created by koscum on 16/06/2021.
//

#include "TimerManager.h"

void TimerManager::registerCallback(const Timer *timer, const std::function<void()> *callback)
{
	auto iterator = callbackDirectory.find((const uintptr_t) timer);
	if (iterator != callbackDirectory.end())
		callbackDirectory[(const uintptr_t) timer].insert(callback);
}

void TimerManager::unregisterCallback(const Timer *timer, const std::function<void()> *callback)
{
	auto iterator = callbackDirectory.find((const uintptr_t) timer);
	if (iterator != callbackDirectory.end())
		callbackDirectory[(const uintptr_t) timer].erase(callback);
}

void TimerManager::registerTimer(const Timer *timer)
{
	auto iterator = callbackDirectory.find((const uintptr_t) timer);
	if (iterator == callbackDirectory.end())
		callbackDirectory[(const uintptr_t) timer] = std::set<const std::function<void()> *>();
}

void TimerManager::unregisterTimer(const Timer *timer)
{
	auto iterator = callbackDirectory.find((const uintptr_t) timer);
	if (iterator != callbackDirectory.end())
		callbackDirectory.erase(iterator);
}

void TimerManager::finish(const Timer *timer)
{
	auto iterator = callbackDirectory.find((const uintptr_t) timer);
	if (iterator != callbackDirectory.end())
		for (const std::function<void()> *callback : callbackDirectory[(const uintptr_t) timer]) callback->operator()();

}

TimerManager *TimerManager::getInstance()
{
	if (!instance) instance = new TimerManager();

	return instance;
}

TimerManager::TimerManager() = default;

TimerManager *TimerManager::instance = nullptr;
