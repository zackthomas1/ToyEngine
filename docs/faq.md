# Frequently Asked Questions (FAQ)

## General Questions

### What is ToyEngine?
ToyEngine is an open-source, educational 3D game engine written in C++ with OpenGL rendering. It's designed to demonstrate modern game engine architecture and graphics programming techniques while being usable for simple projects.

### Who is ToyEngine for?
- **CS Students** interested learning game engine development and studying architecture patterns.
- **Anyone** interested in graphics programming

### Is ToyEngine ready for production use?
ToyEngine is currently in active development. While it demonstrates core engine concepts and can run basic 3D applications, it's not feature-complete for commercial game development. See our [Development Phases](phases.md) for the roadmap.

### What makes ToyEngine different from other engines?
- **Educational focus**: Comprehensive documentation and clear code structure
- **Modern C++**: Uses C++17/20 features and best practices
- **Open source**: MIT license with full source code access
- **Modular design**: Clean separation of concerns and extensible architecture

## Technical Questions

### What platforms does ToyEngine support?
Currently:
- **Windows 10/11** (full support)
- **Linux** (planned for Phase 5)
- **macOS** (planned for Phase 5)
- **Mobile** (planned for Phase 5)

### What graphics APIs does ToyEngine use?
Currently:
- **OpenGL 4.3+** (primary)
- **Vulkan** (planned for Phase 5)
- **DirectX 12** (planned for Phase 5)
- **Metal** (planned for Phase 5)

### What are the system requirements?
**Minimum:**
- Windows 10 or later
- Graphics card with OpenGL 4.3+ support
- 4GB RAM
- Visual Studio 2022 (for development)

**Recommended:**
- Windows 11
- Modern graphics card (GTX 1060/RX 580 or better)
- 8GB RAM
- SSD storage

### What programming languages does ToyEngine use?
- **C++17/20** for the core engine
- **GLSL** for shaders
- **Lua** (planned for scripting)
- **C#** (planned as alternative scripting language)

### Can I use ToyEngine for commercial projects?
Yes! ToyEngine is licensed under the MIT license, which allows commercial use. However, please note that the engine is still in development and may not have all features needed for commercial projects.

## Development Questions

### How do I get started with ToyEngine?
1. Follow the [Quick Start Guide](quick-start.md)
2. Read the [Architecture Documentation](architecture.md)
3. Try the sample projects and tutorials

### How can I contribute to ToyEngine?
There is not a plan for community contribution. Toy Engine is intead as a solo educational project. However, if you are interested in reaching out to learn more about the project feel free to reach out through email. You can find my email address on my GitHub profile. 

### How do I build ToyEngine from source?
```bash
git clone https://github.com/zackthomas1/ToyEngine.git
cd ToyEngine
git submodule update --init --recursive
scripts/Win-GenProjects.bat
# Open ToyEngine.sln in Visual Studio and build
```

### What dependencies does ToyEngine have?
**Included (via submodules):**
- GLFW (windowing)
- GLAD (OpenGL loader)
- GLM (mathematics)
- Dear ImGui (UI)
- spdlog (logging)
- stb (image loading)
- Assimp (3D model loading)

**Required tools:**
- Visual Studio 2022
- Git
- Premake5 (included)

## Usage Questions

### How do I load 3D models?
```cpp
auto model = ToyEngine::Model::Create("path/to/model.obj");
```

Supported formats:
- OBJ (recommended for testing)
- FBX (via Assimp)
- DAE (Collada)
- 3DS
- More formats via Assimp

### How do I create custom shaders?
```cpp
auto shader = ToyEngine::Shader::Create("MyShader", 
    "assets/shaders/vertex.glsl", 
    "assets/shaders/fragment.glsl");
```

### How do I handle input?
```cpp
ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();
if (input.Key(ToyEngine::eKeyCode::kKeyW) != ToyEngine::eKeyState::kRelease) {
    // Handle W key press
}
```

### How do I add lighting to my scene?
```cpp
auto light = ToyEngine::MakeRef<ToyEngine::Light>(ToyEngine::eLightType::kDirectional);
light->m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
light->m_value = glm::vec3(1.0f, 1.0f, 1.0f);
```

### How do I create a custom layer?
```cpp
class MyLayer : public ToyEngine::Layer {
public:
    void OnUpdate(const ToyEngine::TimeStep& timeStep) override {
        // Update logic
    }
    
    void OnEvent(ToyEngine::Event& event) override {
        // Handle events
    }
    
    void OnImGuiRender() override {
        // Draw UI
    }
};
```

## Performance Questions

### How do I optimize performance?
1. **Profile first**: Use Visual Studio Profiler to identify bottlenecks
2. **Reduce draw calls**: Batch similar objects
3. **Optimize shaders**: Reduce complexity in fragment shaders
4. **Use appropriate data structures**: Choose efficient containers
5. **Implement culling**: Don't render objects outside the view

### Why is Debug mode slow?
Debug builds include:
- Additional runtime checks
- Unoptimized code
- Debug symbols
- Verbose logging

Always test performance in Release mode.

### How do I reduce memory usage?
1. **Use smart pointers**: Automatic memory management
2. **Implement object pooling**: Reuse objects instead of creating new ones
3. **Optimize textures**: Use appropriate formats and sizes
4. **Profile memory usage**: Use tools to identify leaks

### How do I improve rendering performance?
1. **Batch draw calls**: Group similar objects
2. **Use instancing**: Render multiple objects efficiently
3. **Implement LOD**: Use simpler models at distance
4. **Optimize shaders**: Reduce fragment shader complexity
5. **Use efficient data formats**: Choose appropriate vertex formats

## Error and Troubleshooting Questions

### The application crashes on startup. What should I do?
1. Run in Debug mode for detailed error information
2. Check the console output for error messages
3. Verify that all assets exist and are accessible
4. Update graphics drivers
5. Check system requirements

See the [Troubleshooting Guide](troubleshooting.md) for more solutions.

### Shaders are not compiling. How do I fix this?
1. Check shader syntax for OpenGL version compatibility
2. Verify that all required uniforms are defined
3. Check for missing #version directives
4. Look at the console output for compilation errors

### Models are not loading. What's wrong?
1. Check that model files exist at the specified path
2. Verify that the model format is supported
3. Check console output for loading errors
4. Try loading a simple model (like a cube) first

### Textures appear black or corrupted. How do I fix this?
1. Verify that texture files exist and are readable
2. Check that texture formats are supported (PNG, JPG, TGA)
3. Ensure texture dimensions are power of 2
4. Check console output for texture loading errors

## Community and Support Questions

### Where can I get help?
- **GitHub Issues**: Bug reports and feature requests
- **Documentation**: Comprehensive guides and API reference
- **Email**: I will respond to email request, but please be specific with your inquiry.  

### Can I use or modify Toy Engine for my own purposes?
Yes, 

### How do I stay updated with development?
- **Watch the GitHub repository** for notifications
- **Follow Youtube vlog series** for weekly/bi-weekly development updates 
- **Check the roadmap** for planned features

### Is ToyEngine good for learning game engine development?
ToyEngine is primarily intended as a personal learning project and experimental platform for graphics programming techniques. As new concepts are explored, they are implemented in ToyEngine to gain hands-on experience. While the engine is educational and open source, some implementations may be basic or use straightforward solutions, rather than production-level approaches.

## Future Development Questions

### What features are planned?
See our [Development Phases](phases.md) for the complete roadmap, including:
- Physically Based Rendering (PBR)
- Animation system
- Physics integration
- Audio system
- Cross-platform support
- Advanced rendering features

### When will feature X be available?
Check the [Development Phases](phases.md) for estimated timelines. Note that development is community-driven and timelines may change based on contributor availability.

## Legal and Licensing Questions

### What license is ToyEngine released under?
ToyEngine is released under the MIT License, which allows:
- Commercial use
- Modification
- Distribution
- Private use

### Can I sell games made with ToyEngine?
Yes, the MIT license allows commercial use without royalties or licensing fees.

### Do I need to credit ToyEngine in my projects?
While not required by the license, the attribution to ToyEngine is appreciated.

---

## Still Have Questions?

If you can't find the answer to your question here:

1. **Search the documentation** - it might be covered elsewhere
2. **Check GitHub Issues** - someone might have asked before
3. **Ask on GitHub Discussions** - the community can help
4. **Create a new issue** - if it's a bug or feature request