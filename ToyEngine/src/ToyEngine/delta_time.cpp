#include "pch.h"
#include "ToyEngine/delta_time.h"
#include <GLFW/glfw3.h>

namespace ToyEngine {
	DeltaTime::DeltaTime()
	{
		current_frame_time_ = glfwGetTime();
		delta_time_ = 0.0f; 
		last_frame_time_ = current_frame_time_;
	}

	void DeltaTime::Step()
	{
		current_frame_time_ = glfwGetTime();
		delta_time_ = current_frame_time_ - last_frame_time_;
		last_frame_time_ = current_frame_time_;
		TY_CORE_TRACE("time step: {}", delta_time_);
	}
}
