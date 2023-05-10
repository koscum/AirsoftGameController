#include "TimerManager.h"

auto TimerManager::registerCallback(Timer *timer, const std::function<void()> *callback) -> void
{
	auto iterator = callbackDirectory.find(timer);
	if (iterator != callbackDirectory.end())
	{
		iterator->second.insert(callback);
	}
}

auto TimerManager::unregisterCallback(Timer *timer, const std::function<void()> *callback) -> void
{
	auto iterator = callbackDirectory.find(timer);
	if (iterator != callbackDirectory.end())
	{
		iterator->second.erase(callback);
	}
}

auto TimerManager::registerTimer(Timer *timer) -> void
{
	auto callbackIterator = callbackDirectory.find(timer);
	if (callbackIterator == callbackDirectory.end())
	{
		callbackDirectory[timer] = std::set<const std::function<void()> *>();
	}
}

auto TimerManager::unregisterTimer(Timer *timer) -> void
{
	auto callbackIterator = callbackDirectory.find(timer);
	if (callbackIterator != callbackDirectory.end())
	{
		callbackDirectory.erase(callbackIterator);
	}
}

auto TimerManager::finish(Timer *timer) -> void
{
	auto iterator = callbackDirectory.find(timer);
	if (iterator != callbackDirectory.end())
	{
		for (const std::function<void()> *callback: iterator->second) callback->operator()();
	}

}

auto TimerManager::tick() -> void
{
	for (auto iterator: callbackDirectory) iterator.first->tick();
}

auto TimerManager::getInstance() -> TimerManager *
{
	if (!instance) instance = new TimerManager();

	return instance;
}

TimerManager::TimerManager() = default;

TimerManager *TimerManager::instance = nullptr;
