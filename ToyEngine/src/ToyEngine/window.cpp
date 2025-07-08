#include "pch.h"
#include "window.h"
#include "ToyEngine/platform/windows/windows_window.h"

namespace ToyEngine {
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef TY_PLATFORM_WINDOWS
		return MakeScope<WindowsWindow>(props);
#else
		TY_CORE_ERROR("Platform Invalid: Window pointer mullptr." );
		return nullptr;
#endif TY_PLATFORM_WINDOWS
	}

	Window::Window(const WindowProps& props)
    {
		// Set the property
		data_.title = props.title;
		data_.width = props.width;
		data_.height = props.height;
		data_.x_mouse_pos = props.x_mouse_pos;
		data_.y_mouse_pos = props.y_mouse_pos;
    }
}