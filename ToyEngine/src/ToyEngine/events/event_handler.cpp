#include "pch.h"
#include "event_handler.h"

namespace ToyEngine
{
	EventHandler::EventHandler() : observer_count_(0)
	{
	}

	void EventHandler::ProcessEvent(Event& event)
	{
		for (int i = 0; i < observer_count_; i++)
		{
			observers_[i]->OnNotify(event);
		}
	}

	void EventHandler::AddObserver(Observer* observer)
	{
		if (observer_count_ < MAXOBSEVERS)
		{
			observers_[observer_count_] = observer;
		}
	}

	void EventHandler::RemoveObserver(Observer* observer)
	{
		for (int i = 0; i < observer_count_; i++)
		{
			if (observers_[i] == observer)
			{
				observers_[i] = nullptr; 
			}
		}
	}
}

