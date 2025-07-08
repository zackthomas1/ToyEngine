#pragma once
namespace ToyEngine
{
	// Defines key codes
	enum class eKeyCode
	{
		kKeyEscape	= 256,
		kKeyW		= 87,
		kKeyS		= 83,
		kKeyA		= 65,
		kKeyD		= 68,
		kKeyE		= 69,
		kKeyQ		= 81,
	};

	enum class eKeyState
	{
		kRelease	= 0,
		kPress		= 1,
		kHold		= 2,
	};

	// defines serveral possible options for camera movement.
	// Use as an abstraction to stay away from window_system specific input methods
	enum class eCameraMovement
	{
		kForward,
		kBackward,
		kLeft,
		kRight,
		kUp,
		kDown,
		kNone,
	};

	//
	enum class eTextureType
	{
		kDiffuse = 1,
		kSpecular = 2,
	};
}
