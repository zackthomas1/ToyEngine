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
		CommandWindowClose(bool* is_running) : is_running_(is_running) {}
		void Execute() override;
	private:
		bool* is_running_;
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