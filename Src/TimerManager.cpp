//
// Created by koscum on 16/06/2021.
//

#include "TimerManager.h"

void TimerManager::registerCallback(Timer *timer, const std::function<void()> *callback)
{
	auto iterator = callbackDirectory.find(timer);
	if (iterator != callbackDirectory.end())
		iterator->second.insert(callback);
}

void TimerManager::unregisterCallback(Timer *timer, const std::function<void()> *callback)
{
	auto iterator = callbackDirectory.find(timer);
	if (iterator != callbackDirectory.end())
		iterator->second.erase(callback);
}

void TimerManager::registerTimer(Timer *timer)
{
	auto callbackIterator = callbackDirectory.find(timer);
	if (callbackIterator == callbackDirectory.end())
		callbackDirectory[timer] = std::set<const std::function<void()> *>();
}

void TimerManager::unregisterTimer(Timer *timer)
{
	auto callbackIterator = callbackDirectory.find(timer);
	if (callbackIterator != callbackDirectory.end())
		callbackDirectory.erase(callbackIterator);
}

void TimerManager::finish(Timer *timer)
{
	auto iterator = callbackDirectory.find(timer);
	if (iterator != callbackDirectory.end())
		for (const std::function<void()> *callback : iterator->second) callback->operator()();

}

void TimerManager::tick()
{
	for (auto iterator : callbackDirectory) iterator.first->tick();
}

TimerManager *TimerManager::getInstance()
{
	if (!instance) instance = new TimerManager();

	return instance;
}

TimerManager::TimerManager() = default;

TimerManager *TimerManager::instance = nullptr;
