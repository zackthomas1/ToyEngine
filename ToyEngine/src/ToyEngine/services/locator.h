#pragma once
#include "ToyEngine/services/time_step.h"
#include "ToyEngine/services/input_poll.h"

namespace ToyEngine
{
	// The Locator is responsible for managing the life time of the service providers.

	class Locator
	{
	public:
		// Enforce static class
		// Delete default, copy, and move constructors
		Locator() = delete;					// Default constructor
		Locator(const Locator&) = delete;	// Copy constructor
		Locator(Locator&&) = delete;		// Move constructor

		// Delete the assignment operators
		Locator& operator=(const Locator&) = delete;
		Locator& operator=(Locator&&) = delete;

		static void DestroyServiceProviders();

		// time step service
		static TimeStep& TimeStepService() { TY_CORE_ASSERT(time_step_, "TimeStep provider not set"); return *time_step_; };
		static void SetTimeStepProvider(TimeStep* time_step);

		// input polling service
		static InputPoll& InputPollService() { TY_CORE_ASSERT(input_poll_, "InputPoll provider not set"); return *input_poll_; }
		static void SetInputPollProvider(InputPoll* input_poll); 
	private:
		static void DeleteTimeStepProvider();
		static void DeleteInputPollProvider();
	private:
		static TimeStep* time_step_;
		static InputPoll* input_poll_;
	};
}