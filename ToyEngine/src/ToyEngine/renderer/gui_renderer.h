#pragma once

#include <GLFW/glfw3.h>

namespace ToyEngine
{
	class GuiRenderer
	{
	public:
		// Enforce static class
		// Delete default, copy, and move constructors
		GuiRenderer() = delete;						// Default constructor
		GuiRenderer(const GuiRenderer&) = delete;	// Copy constructor
		GuiRenderer(GuiRenderer&&) = delete;		// Move constructor

		// Delete the assignment operators
		GuiRenderer& operator=(const GuiRenderer&) = delete;
		GuiRenderer& operator=(GuiRenderer&&) = delete;


		/// <summary>
		/// Initates and configures gui. Creates context, sets io config flags, 
		/// sets gui style, and initializes plateform/renderer backends
		/// </summary>
		/// <param name="window">- </param>
		static void Init(GLFWwindow* window);
		
		/// <summary>
		/// Main render call to draw gui. 
		/// Call in main game loop after the main window render call.
		/// </summary>
		static void DrawGui();
		
		/// <summary>
		/// Clean up imgui. Destroy context and shutdown platform/renderer backendsa
		/// </summary>
		static void Delete();
	};
}