#pragma once
#include "ToyEngine/events/event.h"
#include "ToyEngine/events/observer.h"

namespace ToyEngine
{
	class EventHandler
	{
	public:
		EventHandler();
		~EventHandler() {}

		void ProcessEvent(Event& event);
		void AddObserver(Observer* observer); 
		void RemoveObserver(Observer* observer);

	private: 
		static const int MAXOBSEVERS = 16;
		std::array<Observer*, MAXOBSEVERS> observers_;
		int observer_count_;
	};
}