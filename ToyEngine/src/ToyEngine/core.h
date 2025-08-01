#pragma once
#include <utility>
#include <memory>

#ifdef TY_PLATFORM_WINDOWS
	#ifdef TY_DYNAMIC_LINK
		#ifdef TY_BUILD_DLL
			#define TOYENGINE_API __declspec(dllexport)
		#else
			#define TOYENGINE_API __declspec(dllimport)
		#endif // TY_BUILD_DLL
	#else
		#define TOYENGINE_API
	#endif // TY_DYNAMIC_LINK
#else
	#error Only supports Windows
#endif // TY_PLATFORM_WINDOWS

#ifdef TY_ENABLE_ASSERTS
	#define TY_ASSERT(x, ...) { if(!(x)) { TY_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TY_CORE_ASSERT(x, ...) { if(!(x)) { TY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TY_ASSERT(x, ...)
	#define TY_CORE_ASSERT(x, ...)
#endif

#define TY_BINDFN(fn) [this](auto&& e) { return fn(e); }
//#define TY_BINDFN(fn) std::bind(&fn, this, std::placeholders::_1)
#define TY_BINDFN_ARGS(fn, ...) [this](auto&& e) { return fn(__VA_ARGS__, e); }

constexpr float TY_EPSILON = 0.001;
constexpr int TY_DEFAULT_WINDOW_WIDTH = 800;
constexpr int TY_DEFAULT_WINDOW_HEIGHT = 600;


namespace ToyEngine {
	template <typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	std::shared_ptr<T> MakeRef(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T> 
	using Weak = std::weak_ptr<T>;

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	std::unique_ptr<T> MakeScope(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Vector = std::vector<T>;

	template <typename T, int n>
	using Array = std::array<T, n>;
}