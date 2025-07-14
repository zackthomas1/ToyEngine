#pragma once
#include <cstdint>

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

	// define enum as bit flags (powers of two),
	// Use bitwise operations to check validity.
	enum class eCameraType : uint32_t {
		kFlyCamera = 1 << 0,			// 1
		kOrthographicCamera = 1 << 1,	// 2
		// Add more flags as needed, each one should be a power of two
	};

	//
	enum class eTextureType
	{
		kDiffuse = 1,
		kSpecular = 2,
	};

	enum class eRenderAPI
	{
		kNone	= 0,
		kOpenGL = 1,
	};

	enum class eLightType {
		kDirectional = 1,
		kPoint		= 2,
		kSpot		= 3,
	};
}
