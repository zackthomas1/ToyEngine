# ToyEngine

## Overview

ToyEngine is a 3D game engine that provides essential systems for modern game development including rendering, input handling, scene management, and asset loading. The engine features a modular architecture with well-defined interfaces between systems.

### Purpose 
ToyEngine serves as a foundation for creating 3D applications.

### Key Features
- **Modern OpenGL Rendering**: PBR-based lighting with support for multiple light types
- **Asset Loading**: Support for 3D models (.obj), textures, and materials via Assimp
- **Scene Management**: Hierarchical scene graph with efficient culling
- **Input System**: Comprehensive keyboard and mouse input handling
- **Camera System**: Fly camera with smooth movement and controls
- **Material System**: Shader-based material pipeline with uniform management
- **ImGui Integration**: Built-in debug UI and editor tools
- **Cross-Platform**: Windows support with platform abstraction layer

### Target Hardware
- **Operating System**: Windows 10/11 (Linux support planned)
- **Graphics**: OpenGL 3.3+ compatible GPU
- **Memory**: 4GB RAM minimum, 8GB recommended
- **Storage**: 500MB for engine + assets

## Project Structure 

```
ToyEngine/
├── assets/                    # Runtime assets
│   ├── shaders/              # GLSL shader files
│   ├── textures/             # Image resources
│   └── models/               # 3D model files
├── docs/                     # Documentation and wiki
├── Editor/                   # Editor application
│   ├── src/                  # Editor source code
│   └── premake5.lua          # Editor build configuration
├── ToyEngine/                # Core engine library
│   ├── src/                  # Engine source code
│   │   ├── ToyEngine/        # Core engine systems
│   │   │   ├── events/       # Event system
│   │   │   ├── layers/       # Layer management
│   │   │   ├── renderer/     # Rendering subsystem
│   │   │   ├── model/        # 3D model and mesh handling
│   │   │   ├── platform/     # Platform-specific code
│   │   │   └── services/     # Service locator pattern
│   │   ├── pch.h             # Precompiled header
│   │   └── toy_engine.h      # Main engine header
│   └── vendor/               # Third-party dependencies
├── scripts/                  # Build scripts
├── vendor/                   # External tools (premake)
├── dependencies.lua          # Dependency configuration
├── premake5.lua             # Main build configuration
└── README.md                # This file
```

## Project Setup

### Prerequisites
- **Visual Studio 2022** (Community Edition or higher)
- **Git** for version control
- **Windows 10/11** operating system

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/zackthomas1/ToyEngine.git
   cd ToyEngine
   ```

2. **Initialize submodules**
   ```bash
   git submodule update --init --recursive
   ```

3. **Generate project files**
   ```bash
   # On Windows
   scripts/Win-GenProjects.bat
   
   # Or manually
   vendor/premake/premake5.exe vs2022
   ```

4. **Build the project**
   - Open `ToyEngine.sln` in Visual Studio
   - Set `Editor` as the startup project
   - Build in `Debug` or `Release` configuration

### Running the Engine

After building, run the Editor application to see the engine in action:
- Navigate to `bin/Debug-windows-x86_64/Editor/` or `bin/Release-windows-x86_64/Editor/`
- Run `Editor.exe`

### Dependencies

#### Included Dependencies (via Git Submodules)
- **[Dear ImGui](https://github.com/ocornut/imgui)** - Immediate mode GUI library for debug interfaces
- **[GLAD](https://glad.dav1d.de/)** - OpenGL function loader for managing OpenGL function pointers
- **[GLFW](https://www.glfw.org/)** - Cross-platform window management and input handling
- **[GLM](https://github.com/g-truc/glm)** - Header-only mathematics library for graphics programming
- **[spdlog](https://github.com/gabime/spdlog)** - Fast, header-only C++ logging library
- **[stb](https://github.com/nothings/stb)** - Collection of single-file libraries for image loading
- **[Assimp](https://github.com/assimp/assimp)** - Asset import library for 3D model loading

#### Required Tools
- **[Premake5](https://premake.github.io/)** - Build configuration system (included in `vendor/premake/`)

## Documentation
For detailed documentation, architecture diagrams, and development guides, visit our [Wiki](docs/README.md).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
