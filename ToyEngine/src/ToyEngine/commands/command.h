#pragma once
#include <GLFW/glfw3.h>

namespace ToyEngine
{
	class Command {
	public:
		virtual ~Command() {}
		virtual void Execute() = 0;
	};

	class CommandWindowClose : public Command
	{
	public:
		CommandWindowClose(GLFWwindow* window) : window_(window) {}
		void Execute() override;
	private:
		GLFWwindow* window_;
	};

	class CommandCameraUp : public Command
	{
	public:
		void Execute() override;
	};

	class CommandCameraDown : public Command
	{
	public:
		void Execute() override;
	};
}