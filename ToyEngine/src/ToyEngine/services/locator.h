#pragma once
#include "ToyEngine/services/time_step.h"
#include "ToyEngine/services/time_step_glfw.h"

namespace ToyEngine
{
	class Locator
	{
	public:
		static TimeStep* TimeStepService();
		static void SetTimeStepProvider(TimeStep* time_step);
		static void DeleteTimeStepProvider();
	private:
		static TimeStep* time_step_;
	};
}