#pragma once

namespace ToyEngine
{
	// Service API. The interface the service will expose
	// and that the providers will implement.
	class TimeStep
	{
	public:
		virtual ~TimeStep() {}
		
		virtual void Init() = 0;
		/// <summary>
		/// Increment time step. 
		/// WARNING: Only call this method from the Application::Run().
		/// The time step should only be modified in the main game loop.
		/// </summary>
		virtual void Update() = 0;
		virtual float GetTimeStep() = 0;
	protected: 
		TimeStep() {}
	};

	class NullTimeStep : public TimeStep
	{
	public:
		NullTimeStep() {}
		
		virtual void Init() override
		{
			TY_CORE_WARN( "Platform Invalid. Null service provider - Time step service disabled." );
		}
		virtual void Update() override {}
		virtual float GetTimeStep() override { return 0.0f; }
	};

	// Concrete implementation of the TimeStep service 
	// utilizing GLFW library for 
	// variable time step implementation
	class TimeStepGLFW : public TimeStep
	{
	public:
		TimeStepGLFW() : current_time_(0), last_frame_time_(0), time_step_(0) {}
		~TimeStepGLFW() {}

		virtual void Init() override;
		virtual void Update() override;
		virtual float GetTimeStep() override;
	private:
		float current_time_, last_frame_time_, time_step_;
	};
}