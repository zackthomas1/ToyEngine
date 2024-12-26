#include "pch.h"
#include "locator.h"

namespace ToyEngine
{
	TimeStep* Locator::time_step_ = nullptr; 

	TimeStep* Locator::TimeStepService()
	{
		return time_step_;
	}

	void Locator::SetTimeStepProvider(TimeStep* time_step)
	{
		time_step_ = time_step;
	}

	void Locator::DeleteTimeStepProvider()
	{
		if (time_step_ != nullptr) 
		{
			delete time_step_;
			time_step_ = nullptr;
		}
	}

}
