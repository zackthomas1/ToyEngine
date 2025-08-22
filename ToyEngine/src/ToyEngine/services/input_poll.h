#pragma once
#include "ToyEngine/enum.h"

struct GLFWwindow;

namespace ToyEngine {

	class InputPoll {
	public:
		virtual ~InputPoll() {}
		
		virtual eKeyState Key(eKeyCode key)         const = 0;
		virtual eKeyState Mouse(eMouseCode button)  const = 0;
		virtual float MouseVerticalScroll()         const = 0;
		virtual std::pair<float, float> MousePos()  const = 0;
	protected: 
		InputPoll() {}
	};

	class NullInputPoll : public InputPoll {
	public:
		 NullInputPoll() {}

		virtual eKeyState Key(eKeyCode key)        const override { return eKeyState::kRelease; }
		virtual eKeyState Mouse(eMouseCode button) const override { return eKeyState::kRelease; }
		virtual float MouseVerticalScroll()        const override { return 0.0f; }
		virtual std::pair<float, float> MousePos() const override { return std::pair<float, float>(0.0f, 0.0f); }
	};

	class Window;

	class InputPollGLFW : public InputPoll {
	public:
		InputPollGLFW(Window& window);
		~InputPollGLFW() {}

		virtual eKeyState Key(eKeyCode key)        const override;
		virtual eKeyState Mouse(eMouseCode button) const override;
		virtual float MouseVerticalScroll()        const override;
		virtual std::pair<float, float> MousePos() const override;
	private:
		GLFWwindow* window_ = nullptr;
	};
}