#pragma once
#include <GLFW/glfw3.h>

namespace ToyEngine
{
	// Service API. The interface the service will expose
	// and that the providers will implement.
	class TimeStep
	{
	public:
		virtual ~TimeStep() {}
		
		/// <summary>
		/// Increment time step. 
		/// WARNING: Only call this method from the Application::Run().
		/// The time step should only be modified in the main game loop.
		/// </summary>
		virtual void Update() = 0;
		virtual float GetTimeStep() = 0;
	};
}