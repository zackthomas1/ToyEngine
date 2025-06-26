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
		// If there is currently an active time step provider deallocate 
		// the memory assigned to it before assigning the new provider.
		// This ensures that a memory leak does not occur. 
		if (time_step_ != nullptr) { DeleteTimeStepProvider(); }
		time_step_ = time_step;
	}

	void Locator::DeleteTimeStepProvider()
	{
		// Deallocates memory for time step provider
		TY_CORE_TRACE("Deleted TimeStep Provider");
		if (time_step_ != nullptr) 
		{
			delete time_step_;
			time_step_ = nullptr;
		}
	}

}
