#pragma once
namespace ToyEngine
{
	enum KeyCode
	{
		kKeyEscape = 256, 
		kKeyW = 87, 
		kKeyS = 83,
		kKeyA = 65,
		kKeyD = 68,
		kKeyE = 69,
		kKeyQ = 81,
	};

	enum KeyState
	{
		kPress, 
		kHold, 
		kRelease
	};

	class Event
	{
	public:
		virtual ~Event() {}
	};

	class EventKeyInput : public Event
	{
	public: 
		EventKeyInput(KeyCode key, KeyState state) : key_(key), state_(state) {}
		
		KeyCode GetKeyCode() const { return key_; }
		KeyState GetKeyState() const { return state_; }

	private: 
		KeyCode key_; 
		KeyState state_;
	};

	class EventVerticalScroll : public Event
	{
	public:
		EventVerticalScroll(float y_offset) : y_offset_(y_offset) {}
	private:
		float y_offset_;
	};
}