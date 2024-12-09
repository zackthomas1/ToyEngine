#pragma once
#include "command.h"

namespace ToyEngine
{
	class InputHandler
	{
	public:
		static void ExecuteCommand(Command& command);
	};
}