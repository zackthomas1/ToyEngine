#pragma once

#include <GLFW/glfw3.h>

namespace ToyEngine
{
	class GuiRenderer
	{
	public:
		/// <summary>
		/// Initates and configures gui. Creates context,
		/// sets io config flags, sets gui style, 
		/// and initializes plateform/renderer backends
		/// </summary>
		/// <param name="window"></param>
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