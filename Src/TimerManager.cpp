//
// Created by koscum on 16/06/2021.
//

#include "TimerManager.h"

void TimerManager::registerCallback(const Timer *timer, const std::function<void()> *callback)
{
	auto iterator = callbackDirectory.find(reinterpret_cast<const uintptr_t>(timer));
	if (iterator != callbackDirectory.end())
		iterator->second.insert(callback);
}

void TimerManager::unregisterCallback(const Timer *timer, const std::function<void()> *callback)
{
	auto iterator = callbackDirectory.find(reinterpret_cast<const uintptr_t>(timer));
	if (iterator != callbackDirectory.end())
		iterator->second.erase(callback);
}

void TimerManager::registerTimer(const TIM_HandleTypeDef *handle, Timer *timer)
{
	auto timerIterator = timerDirectory.find(reinterpret_cast<const uintptr_t>(handle));
	if (timerIterator == timerDirectory.end())
		timerDirectory[reinterpret_cast<const uintptr_t>(handle)] = std::set<Timer *>();
	timerDirectory[reinterpret_cast<const uintptr_t>(handle)].insert(timer);

	auto callbackIterator = callbackDirectory.find(reinterpret_cast<const uintptr_t>(timer));
	if (callbackIterator == callbackDirectory.end())
		callbackDirectory[reinterpret_cast<const uintptr_t>(timer)] = std::set<const std::function<void()> *>();
}

void TimerManager::unregisterTimer(const TIM_HandleTypeDef *handle, Timer *timer)
{
	auto callbackIterator = callbackDirectory.find(reinterpret_cast<uintptr_t>(timer));
	if (callbackIterator != callbackDirectory.end())
		callbackDirectory.erase(callbackIterator);

	auto timerIterator = timerDirectory.find(reinterpret_cast<const uintptr_t>(handle));
	if (timerIterator != timerDirectory.end())
		timerIterator->second.erase(timer);
}

void TimerManager::finish(const Timer *timer)
{
	auto iterator = callbackDirectory.find(reinterpret_cast<const uintptr_t>(timer));
	if (iterator != callbackDirectory.end())
		for (const std::function<void()> *callback : iterator->second) callback->operator()();

}

void TimerManager::tick(const TIM_HandleTypeDef *handle)
{
	auto timerIterator = timerDirectory.find(reinterpret_cast<const uintptr_t>(handle));
	if (timerIterator != timerDirectory.end())
		for (Timer *timer : timerIterator->second) timer->tick();
}

TimerManager *TimerManager::getInstance()
{
	if (!instance) instance = new TimerManager();

	return instance;
}

TimerManager::TimerManager() = default;

TimerManager *TimerManager::instance = nullptr;
