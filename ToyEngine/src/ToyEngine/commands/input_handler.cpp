#include "pch.h"

#include "input_handler.h"

namespace ToyEngine
{
	void InputHandler::ExecuteCommand(Command& command)
	{
		command.Execute();
	}

}