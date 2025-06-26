#pragma once
#include "event.h"

namespace ToyEngine
{
	class Observer
	{
	public:
		virtual ~Observer() {}
		virtual void OnEvent(Event& event) = 0;
	};
}