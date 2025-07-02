#include "pch.h"
#include "locator.h"

namespace ToyEngine
{
	// Service provider pointers
	TimeStep* Locator::time_step_	= nullptr;
	InputPoll* Locator::input_poll_	= nullptr;

	// Time Step service methods
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

	// Input Poll service methods
	InputPoll* Locator::InputPollService()
	{
		return input_poll_;
	}

	void Locator::SetInputPollProvider(InputPoll* input_poll)
	{
		if (input_poll_ != nullptr) { DeleteInputPollProvider(); }
		input_poll_ = input_poll;
	}

	void Locator::DeleteInputPollProvider()
	{
		TY_CORE_TRACE("Deleted InputPoll Provider");
		if (input_poll_ != nullptr)
		{
			delete input_poll_;
			input_poll_ = nullptr;
		}
	}
}