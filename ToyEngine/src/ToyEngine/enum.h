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


	enum class eLightType {
		kDirectional = 1,
		kPoint = 2,
		kSpot = 3,
	};

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

	enum class eBufferType
	{
		kARRAY_BUFFER			= 0x8892,
		kELEMENT_ARRAY_BUFFER	= 0x8893,
		kFRAME_BUFFER,
	};

	enum class eDataType {
		kFLOAT = 0x1406,
	};

	enum class ePrimType {
		kTRIANGLE = 0x0004, 
	};

	enum class eParamType 
	{
		kDEPTH_WRITEMASK	= 0x0B72,
		kCULL_FACE			= 0x0B44,
		kDEPTH_FUNC			= 0x0B74,
	};

	enum class eDepthFunc {
		kNEVER = 0x0200,    // GL_NEVER
		kLESS = 0x0201,     // GL_LESS  
		kEQUAL = 0x0202,    // GL_EQUAL
		kLEQUAL = 0x0203,   // GL_LEQUAL
		kGREATER = 0x0204,  // GL_GREATER
		kNOTEQUAL = 0x0205, // GL_NOTEQUAL
		kGEQUAL = 0x0206,   // GL_GEQUAL
		kALWAYS = 0x0207    // GL_ALWAYS
	};
}
