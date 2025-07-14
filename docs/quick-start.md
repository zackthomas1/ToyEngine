# Quick Start Guide

Get up and running with ToyEngine in just a few minutes!

## Prerequisites

Before you begin, ensure you have the following installed:

- **Windows 10/11** (Linux support coming soon)
- **Visual Studio 2022** (Community Edition or higher)
- **Git** for version control
- **Graphics drivers** that support OpenGL 4.3+

## 1. Clone the Repository

```bash
git clone https://github.com/zackthomas1/ToyEngine.git
cd ToyEngine
```

## 2. Initialize Submodules

ToyEngine uses Git submodules for dependencies. Initialize them with:

```bash
git submodule update --init --recursive
```

## 3. Generate Project Files

Run the project generation script:

```bash
# Windows
scripts/Win-GenProjects.bat

# Or manually
vendor/premake/premake5.exe vs2022
```

This will create `ToyEngine.sln` in the root directory.

## 4. Build the Project

1. Open `ToyEngine.sln` in Visual Studio 2022
2. Set the solution configuration to `Debug` or `Release`
3. Set `Editor` as the startup project (right-click → Set as Startup Project)
4. Build the solution (`Ctrl+Shift+B` or Build → Build Solution)

## 5. Run the Engine

After building successfully:

1. Press `F5` to run the Editor application
2. You should see a window with a 3D scene containing sample models
3. Use `WASD` keys to move the camera
4. Use the mouse to look around
5. Press `E` and `Q` to move up and down

## 6. Explore the Interface

The Editor includes:

- **3D Viewport**: Main rendering area showing the scene
- **ImGui Demo Window**: Example of ImGui interface capabilities
- **Hello World Window**: Engine controls and scene parameters
- **Light Controls**: Adjust lighting parameters in real-time

## 7. Understanding the Code

The main entry point is in `Editor/src/editor.cpp`:

```cpp
class Editor : public ToyEngine::Application
{
public:
    Editor()
    {
        PushLayer(new Scene());
    }
};
```

The `Scene` class demonstrates:
- Loading 3D models
- Setting up lighting
- Handling input
- Rendering with shaders

## 8. Making Your First Changes

Try modifying the scene:

1. Open `Editor/src/editor.cpp`
2. Look for the `OnAttach()` method in the `Scene` class
3. Try changing the model paths or adding new models
4. Rebuild and run to see your changes

## Common Issues

### Build Errors
- Ensure all submodules are properly initialized
- Check that you're using Visual Studio 2022
- Verify that the `Editor` project is set as the startup project

### Runtime Errors
- Make sure your graphics drivers support OpenGL 4.3+
- Check that asset files exist in the `assets/` directory
- Verify that the working directory is set correctly

### Performance Issues
- Try switching to `Release` configuration for better performance
- Update your graphics drivers
- Check that your GPU supports the required OpenGL version

## Next Steps

Now that you have ToyEngine running:

1. **Read the Architecture Guide**: Understand how the engine is structured
2. **Explore the API Documentation**: Learn about available classes and functions
3. **Try the Tutorials**: Follow step-by-step guides to create your own content
4. **Join the Community**: Contribute to the project or ask questions

## Useful Commands

```bash
# Regenerate project files
vendor/premake/premake5.exe vs2022

# Clean build directory
vendor/premake/premake5.exe clean

# Generate for different IDE (future)
vendor/premake/premake5.exe vs2019
vendor/premake/premake5.exe gmake2
```

## Project Structure Quick Reference

```
ToyEngine/
├── Editor/           # Editor application
├── ToyEngine/        # Core engine library
├── assets/           # Runtime assets (models, textures, shaders)
├── docs/             # Documentation
├── scripts/          # Build scripts
└── vendor/           # Third-party dependencies
```

Happy coding! 🚀