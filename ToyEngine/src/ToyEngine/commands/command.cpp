#include "pch.h"
#include "command.h"

namespace ToyEngine
{
	void ToyEngine::CommandWindowClose::Execute()
	{
		TY_CORE_TRACE("Command - Close Window");
	}
	void ToyEngine::CommandCameraForward::Execute()
	{
		TY_CORE_TRACE("Command - Camera Forward");
	}
	void ToyEngine::CommandCameraBackwards::Execute()
	{
		TY_CORE_TRACE("Command - Camera Backwards");
	}

}
