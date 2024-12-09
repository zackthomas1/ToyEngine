#include "pch.h"
#include "command.h"

namespace ToyEngine
{
	void ToyEngine::CommandWindowClose::Execute()
	{
		TY_CORE_TRACE("Command - Close Window");
		glfwSetWindowShouldClose(window_, true);

	}
	void ToyEngine::CommandCameraUp::Execute()
	{
		TY_CORE_TRACE("Command - Camera Up");
	}
	void ToyEngine::CommandCameraDown::Execute()
	{
		TY_CORE_TRACE("Command - Camera Down");
	}

}
