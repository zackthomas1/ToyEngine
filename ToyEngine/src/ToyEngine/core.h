#pragma once

#ifdef TY_DEBUG
	#define TY_ENABLE_ASSERTS
#endif // TY_DEBUG

#ifdef TY_ENABLE_ASSERTS
	#define TY_ASSERT(x, ...) { if(!(x)) { TY_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TY_CORE_ASSERT(x, ...) { if(!(x)) { TY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TY_ASSERT(x, ...)
	#define TY_CORE_ASSERT(x, ...)
#endif
