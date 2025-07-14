# Frequently Asked Questions (FAQ)

## General Questions

### What is ToyEngine?
ToyEngine is an open-source, educational 3D game engine written in C++ with OpenGL rendering. It's designed to demonstrate modern game engine architecture and graphics programming techniques while being usable for real projects.

### Who is ToyEngine for?
- **Students** learning game engine development
- **Educators** teaching graphics programming
- **Indie developers** creating 3D games
- **Engine developers** studying architecture patterns
- **Anyone** interested in graphics programming

### Is ToyEngine ready for production use?
ToyEngine is currently in active development. While it demonstrates core engine concepts and can run basic 3D applications, it's not yet feature-complete for commercial game development. See our [Development Phases](phases.md) for the roadmap.

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
4. Join the community and ask questions

### How can I contribute to ToyEngine?
See our [Contributing Guide](contributing.md) for detailed information on:
- Code contributions
- Bug reports
- Documentation improvements
- Feature requests

### What should I work on as a first contribution?
Look for GitHub issues labeled:
- `good first issue`
- `help wanted`
- `documentation`
- `beginner friendly`

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

### How do I add a new feature to ToyEngine?
1. **Plan**: Discuss the feature in GitHub issues
2. **Design**: Create a design document if needed
3. **Implement**: Follow coding standards and write tests
4. **Document**: Update documentation and examples
5. **Submit**: Create a pull request with your changes

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
- **GitHub Discussions**: Questions and general discussion
- **Documentation**: Comprehensive guides and API reference
- **Community Forums**: Connect with other users

### How do I report a bug?
1. Search existing issues to avoid duplicates
2. Create a new issue with:
   - Clear description of the problem
   - Steps to reproduce
   - Expected vs actual behavior
   - System information
   - Screenshots or logs if applicable

### How do I request a feature?
1. Check existing feature requests
2. Create a new issue with:
   - Clear description of the feature
   - Use case and rationale
   - Potential implementation ideas
   - Alternatives considered

### How do I stay updated with development?
- **Watch the GitHub repository** for notifications
- **Follow release notes** for new features
- **Join community discussions** for latest news
- **Check the roadmap** for planned features

## Educational Questions

### Is ToyEngine good for learning game engine development?
Yes! ToyEngine is specifically designed for educational purposes with:
- Clear, well-documented code
- Comprehensive architecture documentation
- Step-by-step tutorials
- Examples and sample projects

### What can I learn from ToyEngine?
- Modern C++ practices and design patterns
- OpenGL graphics programming
- Game engine architecture
- Real-time rendering techniques
- Asset pipeline development
- Performance optimization

### Are there tutorials available?
Yes, we provide:
- Getting started guides
- Architecture explanations
- API documentation with examples
- Sample projects
- Video tutorials (planned)

### Can I use ToyEngine for academic projects?
Absolutely! ToyEngine is perfect for:
- Computer graphics courses
- Game development classes
- Software engineering projects
- Research in real-time rendering
- Master's/PhD thesis work

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

### How can I influence the development roadmap?
- **Participate in discussions** about features
- **Submit feature requests** with detailed use cases
- **Contribute code** for features you need
- **Provide feedback** on existing features

### Will ToyEngine support [specific feature]?
Check the roadmap first. If it's not listed:
1. Create a feature request issue
2. Discuss the feature with the community
3. Consider contributing the feature yourself
4. Wait for community/maintainer feedback

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
While not required by the license, we appreciate attribution to ToyEngine and its contributors.

### Can I contribute to ToyEngine if I work for a company?
Yes, but check your employment contract for any restrictions on contributing to open source projects.

---

## Still Have Questions?

If you can't find the answer to your question here:

1. **Search the documentation** - it might be covered elsewhere
2. **Check GitHub Issues** - someone might have asked before
3. **Ask on GitHub Discussions** - the community can help
4. **Create a new issue** - if it's a bug or feature request

We're here to help and always happy to answer questions!