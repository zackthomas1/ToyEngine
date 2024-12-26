#pragma once
#include "ToyEngine/services/time_step.h"

namespace ToyEngine
{
	// Concrete implementation of the TimeStep service 
	// utilizing GLFW library
	class TimeStepGLFW : public TimeStep
	{
	public:
		TimeStepGLFW(); 
		~TimeStepGLFW() {}

		virtual void Update() override;
		virtual float GetTimeStep() override;
	private:
		float current_time_, last_frame_time_, time_step_;
	};
}