#pragma once
#include "pch.h"

#include "core.h"


namespace ToyEngine
{
	struct WindowProps
	{
		std::string title_;
		unsigned int width_;
		unsigned int height_;

		WindowProps(const std::string& title = "Learn OpenGL",
			unsigned int width = 800,
			unsigned int height = 600)
			: title_(title), width_(width), height_(height) {}
	};

	class TOYENGINE_API Window
	{
	public: 
		Window(); 
		virtual ~Window(); 

		static Window* Create();
	private:
		void Init();

	private:
		//GLFWwindow* window_;
	};
}