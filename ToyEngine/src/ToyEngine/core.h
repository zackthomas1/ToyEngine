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
  /// @brief Assertion macro that checks a condition and logs an error message if the condition is false.
  #define TY_ASSERT(x, ...) { if(!(x)) { TY_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

  /// @brief Core assertion macro that checks a condition and logs an error message if the condition is false.
  #define TY_CORE_ASSERT(x, ...) { if(!(x)) { TY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
  #define TY_ASSERT(x, ...)
  #define TY_CORE_ASSERT(x, ...)
#endif

/// @brief Macro to bind a member function to an event handler, capturing the current instance (this) and forwarding the event parameter.
#define TY_BINDFN(fn) [this](auto&& e) { return fn(e); }

//#define TY_BINDFN(fn) std::bind(&fn, this, std::placeholders::_1)

/// @brief Macro to bind a member function to an event handler with additional arguments, capturing the current instance (this) and forwarding the event parameter.
#define TY_BINDFN_ARGS(fn, ...) [this](auto&& e) { return fn(__VA_ARGS__, e); }

/* Constant value for floating-point comparisons to account for precision errors. */
constexpr float TY_EPSILON = 0.001f;

/* Constant value that defines the default width of the application window. */
constexpr int TY_DEFAULT_WINDOW_WIDTH = 1280;

/* Constant value that defines the default height of the application window. */
constexpr int TY_DEFAULT_WINDOW_HEIGHT = 720;

namespace ToyEngine {
  /// @brief Type alias for a shared pointer to an object of type T.
  template <typename T>
  using Ref = std::shared_ptr<T>;

  /// @brief Creates a shared pointer to an object of type T, forwarding the provided arguments to the constructor of T.
  /// @tparam T: The type of the object to create.
  /// @tparam Args: The types of the arguments to pass to the constructor of T.
  /// @param args: The arguments to pass to the constructor of T.
  /// @return A shared pointer to the newly created object of type T.
  template<typename T, typename... Args>
  std::shared_ptr<T> MakeRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  /// @brief Type alias for a weak pointer to an object of type T.
  template <typename T>
  using Weak = std::weak_ptr<T>;

  /// @brief Type alias for a unique pointer to an object of type T.
  template <typename T>
  using Scope = std::unique_ptr<T>;

  /// @brief Creates a unique pointer to an object of type T, forwarding the provided arguments to the constructor of T.
  /// @tparam T: The type of the object to create.
  /// @tparam Args: The types of the arguments to pass to the constructor of T.
  /// @param args: The arguments to pass to the constructor of T.
  /// @return A unique pointer to the newly created object of type T.
  template<typename T, typename... Args>
  std::unique_ptr<T> MakeScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  /// @brief Type alias for a vector (dynamic array) of objects of type T.
  template <typename T>
  using Vector = std::vector<T>;

  /// @brief Type alias for a fixed-size array of objects of type T with size n.
  template <typename T, int n>
  using Array = std::array<T, n>;
}