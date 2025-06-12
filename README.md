# Toy Engine

## Project Structure 
```
ToyEngine/
├── assets/
│   ├── shaders/
│   ├── textures/
├── bin/
├── bin-int/
├── Sandbox/
│   ├── src/
│   ├── premake5.lua
│   ├── Sandbox.vcxproj
│   └── Sandbox.vcxproj.user
├── scripts/
├── ToyEngine/
│   ├── src/
│       ├── ToyEngine/
│           ├── events/
│           ├── layers/
│           ├── renderer/
│               ├── camera/
│               ├── mesh/
│               ├── texture/
│           ├── services
│           ├── application.cpp
│           ├── application.h
│           ├── core.h
│           ├── entry_point.h
│           ├── log.cpp
│           ├── log.h
│           ├── windows_window.cpp
|           └── windows_window.h
│       ├── pch.cpp
│       ├── pch.h
│       └── toy_engine.h
│   ├── vendor/
│       ├── bin/
│       ├── bin-int/
│       ├── dearimgui
│       ├── glad
│       ├── GLFW
│       ├── glm
│       ├── spdlog
|       └── stb

│   ├── ToyEngine.vcxproj
│   ├── ToyEngine.vcxproj.filters
│   └── ToyEngine.vcxproj.user
├── vendor/
├── .gitignore
├── .gitmodules
├── dependencies.lua
├── LICENSE
├── premake5.lua
├── README.md 
└── ToyEngine.sln 

## Dependencies
Included Dependencies 
- [dearimgui](https://github.com/ocornut/imgui) - graphical user interface birbary for C++
- [glad](https://glad.dav1d.de/) - Generates loader for OpenGL functions.  Location of OpenGL functions not known at compile-time and needs to be queried at run-time.
  manages function pointers for OpenGl, which is necessary beccause driver manufacturers implement OpenGL specification differently  
- [GLFW](https://www.glfw.org/) - Open Source, multi-platform library for OpenGL development on the desktop.
  Provides a simple API for creating windows, contexts and surfaces, receiving input and events
- [glm](https://github.com/g-truc/glm) - header only C++ mathematics library for graphics software based on GLSL specification
- [spdlog](https://github.com/gabime/spdlog) - Fast C++ logging library
- [stb](https://github.com/nothings/stb) - for image loading

Non-Included Dependencies
[Premake](https://premake.github.io/) - C++ project build configuration
