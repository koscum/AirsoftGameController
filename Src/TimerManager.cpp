//
// Created by koscum on 16/06/2021.
//

#include "TimerManager.h"

void TimerManager::registerCallback(const Timer *timer, const std::function<void()> *callback)
{
	for (Entry *subscription : entries)
	{
		if (subscription->timer != timer) continue;

		subscription->callbacks.push_back(callback);

		break;
	}
}

void TimerManager::unregisterCallback(const Timer *timer, const std::function<void()> *callback)
{
	for (Entry *subscription : entries)
	{
		if (subscription->timer != timer) continue;

		subscription->callbacks.remove(callback);

		break;
	}
}

void TimerManager::registerTimer(const Timer *timer)
{
	entries.push_back(new Entry(timer));
}

void TimerManager::unregisterTimer(const Timer *timer)
{
	for (Entry *entry : entries)
	{
		if (entry->timer != timer) continue;

		entries.remove(entry);
		delete entry;

		break;
	}
}

void TimerManager::finish(const Timer *timer)
{
	for (Entry *entry : entries)
	{
		if (entry->timer != timer) continue;

		for (const std::function<void()> *callback : entry->callbacks) callback->operator()();

		break;
	}
}

TimerManager *TimerManager::getInstance()
{
	if (!instance) instance = new TimerManager();

	return instance;
}

TimerManager::TimerManager() {}

TimerManager *TimerManager::instance = 0L;
