#pragma once

#ifdef TY_BUILD_DLL
	#define TOYENGINE_API __declspec(dllexport)
#else
	#define TOYENGINE_API __declspec(dllimport)
#endif
