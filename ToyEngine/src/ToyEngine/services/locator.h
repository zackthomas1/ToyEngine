#pragma once
#include "ToyEngine/services/time_step.h"
#include "ToyEngine/services/time_step_glfw.h"
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

		// time step service
		static TimeStep* TimeStepService();
		static void SetTimeStepProvider(TimeStep* time_step);
		static void DeleteTimeStepProvider();

		// Input polling service
		static InputPoll* InputPollService();
		static void SetInputPollProvider(InputPoll* input_poll); 
		static void DeleteInputPollProvider();
		
	private:
		static TimeStep* time_step_;
		static InputPoll* input_poll_;
	};
}