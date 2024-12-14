#pragma once

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
		CommandWindowClose() {}
		void Execute() override;
	};

	class CommandCameraForward : public Command
	{
	public:
		CommandCameraForward() {}
		void Execute() override;
	};

	class CommandCameraBackwards : public Command
	{
	public:
		CommandCameraBackwards() {}
		void Execute() override;
	};
}