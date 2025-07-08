#include "pch.h"
#include "ToyEngine/services/time_step.h"
#include <GLFW/glfw3.h>

namespace ToyEngine
{
	void TimeStepGLFW::Init()
	{
		TY_CORE_INFO("GLFW time step provider");
		current_time_ = static_cast<float>(glfwGetTime());
		last_frame_time_ = current_time_;
		time_step_ = 0.0f;
	}

	void TimeStepGLFW::Update()
	{
		current_time_ = static_cast<float>(glfwGetTime());
		time_step_ = current_time_ - last_frame_time_;
		last_frame_time_ = current_time_;
	}

	float TimeStepGLFW::GetTimeStep()
	{
		return time_step_;
	}
}
