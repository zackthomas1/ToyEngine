#pragma once
namespace ToyEngine
{
	enum eKeyCode
	{
		kKeyEscape = 256,
		kKeyW = 87,
		kKeyS = 83,
		kKeyA = 65,
		kKeyD = 68,
		kKeyE = 69,
		kKeyQ = 81,
	};

	enum eKeyState
	{
		kRelease = 0,
		kPress = 1,
		kHold = 2,
	};
}
