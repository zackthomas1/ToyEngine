# ToyEngine Copilot Instructions

## Project Overview
ToyEngine is a modern C++ game engine with OpenGL rendering, featuring a modular architecture with service locator pattern, event-driven layer system, and PBR rendering pipeline.

## Architecture Patterns

### Service Locator Pattern
- **Core Implementation**: `ToyEngine/src/ToyEngine/services/locator.h/cpp`
- **Static Services**: TimeStep, InputPoll accessed via `Locator::TimeStepService()`, `Locator::InputPollService()`
- **Platform Abstraction**: Services have Windows-specific implementations (e.g., `TimeStepGLFW`, `InputPollGLFW`)
- **Initialization**: Services must be initialized via `Locator::XxxService().Init()` before use

### Event System
- **Base Class**: `Event` in `ToyEngine/src/ToyEngine/event.h`
- **Dispatcher Pattern**: Use `EventDispatcher` to route specific event types to handlers
- **Event Forwarding**: Many classes have `OnEvent(Event& e)` methods that dispatch to specific handlers using `TY_BINDFN` macro
- **Layer Propagation**: Events propagate from topmost layer to background until handled

### Layer System
- **Base**: `Layer` class with lifecycle methods: `OnAttach()`, `Update()`, `OnImGuiRender()`, `OnEvent()`
- **Stack Management**: `LayerStack` manages layers and overlays, with overlays rendered last
- **Main Loop**: Application calls `Update()` and `OnImGuiRender()` on all layers each frame

## Critical Build System

### Premake5 Configuration
- **Generate Projects**: Run `scripts/Win-GenProjects.bat` or `vendor/premake/premake5.exe vs2022`
- **Dependencies**: Managed via `dependencies.lua` and included submodules
- **Configurations**: Debug (with asserts), Release, Dist
- **Platform Defines**: `TY_PLATFORM_WINDOWS`, `TY_PLATFORM_OPENGL` control conditional compilation

### Platform Abstraction
- **Render API**: Factory pattern in `ToyEngine/src/ToyEngine/renderer/` creates OpenGL-specific implementations
- **Window Management**: `Window::Create()` returns platform-specific window (currently Windows/GLFW)
- **Example Pattern**: `VertexArray::Create()` → `OpenGLVertexArray` based on `Renderer::API()`

## Rendering Architecture

### Resource Management
- **Smart Pointers**: Use `Ref<T>` (shared_ptr) for shared resources, `Scope<T>` (unique_ptr) for owned resources
- **Shader System**: `ShaderLibrary` manages shader compilation and storage by name
- **Vertex Data**: `Vertex` struct (32 bytes: Position, Normal, TexCoords), managed by `VertexArray`/`VertexBuffer`

### Material and Mesh System
- **PBR Materials**: Support diffuse/specular textures, environment maps, roughness/metallic parameters
- **Scene Graph**: `SceneNode` hierarchy for transforms and object management
- **Asset Loading**: Assimp integration for 3D models (.obj files primarily)

## Key Conventions

### Memory Management
- **Factory Functions**: Always return smart pointers (`Ref<T>` for shared, `Scope<T>` for unique)
- **API Abstraction**: Create abstract base classes with static `Create()` methods that return platform implementations

### Naming Conventions
- **Members**: `member_` (trailing underscore for private members)
- **Macros**: `TY_` prefix (e.g., `TY_CORE_ASSERT`, `TY_BINDFN`)
- **Enums**: `eCamelCase` prefix (e.g., `eRenderAPI::kOpenGL`)

### Logging System
- **Core Engine**: `TY_CORE_INFO()`, `TY_CORE_WARN()`, `TY_CORE_ERROR()` 
- **Client App**: `TY_INFO()`, `TY_WARN()`, `TY_ERROR()`
- **Backend**: spdlog with platform-specific console sinks

## Editor Integration

### ImGui Integration
- **Layer System**: `ImGuiLayer` handles Dear ImGui setup/teardown
- **Viewport**: Editor viewport renders to framebuffer, displays in ImGui window
- **Event Handling**: `Viewport` class transforms ImGui events to world-space for camera controller

### Scene Management
- **Editor Class**: Inherits from `Application`, contains `Scene` layer with viewport
- **Camera System**: Strategy pattern with `FlyCameraStrategy`, `OrbitCameraStrategy`, `OrthoCameraStrategy`
- **Debug UI**: Scene hierarchy viewer using `DrawSceneNodeTree()` recursive function

## Common Gotchas

### Initialization Order
1. Must call `Locator::TimeStepService().Init()` and `Locator::InputPollService().Init()` before use
2. Renderer must be initialized via `Renderer::Init()` before creating graphics resources
3. ImGui layer must be pushed to layer stack for GUI to work

### Event System
- Events propagate from top layer to bottom until `SetEventHandled(true)` is called
- Use `TY_BINDFN` macro for member function binding in `EventDispatcher::Dispatch()`
- Camera strategies expect normalized device coordinates from viewport transformations

### Asset Paths
- Assets loaded relative to working directory (typically where executable runs)
- Shaders in `assets/shaders/`, models in `assets/models/`, textures in `assets/textures/`
