#pragma once
#include "event.h"

namespace ToyEngine
{
	class Observer
	{
	public:
		virtual ~Observer() {}
		virtual void OnNotify(Event& event) = 0;
	};
}