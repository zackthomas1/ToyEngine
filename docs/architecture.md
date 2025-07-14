# System Architecture

This document describes the high-level architecture of ToyEngine, including system diagrams and component interactions.

## Architecture Overview

ToyEngine follows a layered architecture with clear separation of concerns between different subsystems. The engine is designed with modularity and extensibility in mind.

## C4 Architecture Diagrams

### Context Diagram (Level 1)

```
┌─────────────────────────────────────────────────────────────────────────┐
│                              ToyEngine                                  │
│                         3D Game Engine                                 │
│                                                                         │
│  Provides rendering, input, scene management, and asset loading        │
│  capabilities for 3D applications and games                           │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                            Game Developer                               │
│                                                                         │
│  Uses ToyEngine APIs to create 3D applications, games, and            │
│  interactive experiences                                               │
└─────────────────────────────────────────────────────────────────────────┘
```

### Container Diagram (Level 2)

```
┌─────────────────────────────────────────────────────────────────────────┐
│                              ToyEngine System                           │
│                                                                         │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    │
│  │     Editor      │    │   ToyEngine     │    │     Assets      │    │
│  │  Application    │    │  Core Library   │    │   Directory     │    │
│  │                 │    │                 │    │                 │    │
│  │  - ImGui UI     │◄──►│  - Renderer     │◄──►│  - 3D Models    │    │
│  │  - Scene View   │    │  - Input System │    │  - Textures     │    │
│  │  - Debug Tools  │    │  - Event System │    │  - Shaders      │    │
│  └─────────────────┘    │  - Scene Graph  │    │  - Materials    │    │
│                         │  - Resource Mgr │    └─────────────────┘    │
│                         └─────────────────┘                           │
│                                  │                                    │
│                                  ▼                                    │
│  ┌─────────────────────────────────────────────────────────────────┐  │
│  │                    Platform Layer                               │  │
│  │                                                                 │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐            │  │
│  │  │   Windows   │  │   OpenGL    │  │  File I/O   │            │  │
│  │  │   Window    │  │  Graphics   │  │   System    │            │  │
│  │  │   (GLFW)    │  │   (GLAD)    │  │             │            │  │
│  │  └─────────────┘  └─────────────┘  └─────────────┘            │  │
│  └─────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────┘
```

### Component Diagram (Level 3)

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        ToyEngine Core Components                        │
│                                                                         │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    │
│  │   Application   │    │  Event System   │    │  Layer Stack    │    │
│  │                 │    │                 │    │                 │    │
│  │  - Run Loop     │◄──►│  - Dispatcher   │◄──►│  - ImGui Layer  │    │
│  │  - Window Mgmt  │    │  - Event Types  │    │  - Scene Layer  │    │
│  │  - Layer Mgmt   │    │  - Handlers     │    │  - Custom Layers│    │
│  └─────────────────┘    └─────────────────┘    └─────────────────┘    │
│           │                       │                       │           │
│           │                       │                       │           │
│           ▼                       ▼                       ▼           │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    │
│  │   Input System  │    │   Renderer      │    │ Resource Manager│    │
│  │                 │    │                 │    │                 │    │
│  │  - Input Poll   │    │  - Cameras      │    │  - Asset Loader │    │
│  │  - Key/Mouse    │    │  - Shaders      │    │  - File System  │    │
│  │  - Event Gen    │    │  - Materials    │    │  - Caching      │    │
│  └─────────────────┘    │  - Meshes       │    └─────────────────┘    │
│                         │  - Lighting     │                           │
│                         │  - Scene Graph  │                           │
│                         └─────────────────┘                           │
│                                  │                                    │
│                                  ▼                                    │
│  ┌─────────────────────────────────────────────────────────────────┐  │
│  │                    Graphics Pipeline                            │  │
│  │                                                                 │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐            │  │
│  │  │   Vertex    │  │  Fragment   │  │  Uniform    │            │  │
│  │  │  Processing │  │  Shading    │  │  Management │            │  │
│  │  │             │  │             │  │             │            │  │
│  │  └─────────────┘  └─────────────┘  └─────────────┘            │  │
│  └─────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────┘
```

## System Flow Diagram

```
┌─────────────┐
│   Start     │
│ Application │
└─────────────┘
      │
      ▼
┌─────────────┐
│ Initialize  │
│  Systems    │
│ (Window,    │
│ Renderer,   │
│ Input)      │
└─────────────┘
      │
      ▼
┌─────────────┐
│ Load Assets │
│ (Models,    │
│ Textures,   │
│ Shaders)    │
└─────────────┘
      │
      ▼
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│ Game Loop   │───►│ Poll Input  │───►│ Update      │
│ (Running)   │    │ Events      │    │ Camera      │
└─────────────┘    └─────────────┘    └─────────────┘
      ▲                                      │
      │                                      ▼
      │              ┌─────────────┐    ┌─────────────┐
      │              │ Present     │    │ Update      │
      │              │ Frame       │    │ Scene       │
      │              │ (Swap       │    │ Objects     │
      │              │ Buffers)    │    └─────────────┘
      │              └─────────────┘           │
      │                     ▲                 ▼
      │                     │            ┌─────────────┐
      │              ┌─────────────┐    │ Render      │
      │              │ Draw ImGui  │    │ Scene       │
      │              │ Interface   │    │ (Geometry,  │
      │              └─────────────┘    │ Lighting)   │
      │                     ▲          └─────────────┘
      │                     │                 │
      └─────────────────────┴─────────────────┘
```

## UML Class Diagram - Core Components

```
┌─────────────────────────────────────────────────────────────────────────┐
│                           Core Engine Classes                           │
│                                                                         │
│  ┌─────────────────┐                    ┌─────────────────┐            │
│  │   Application   │                    │      Event      │            │
│  │─────────────────│                    │─────────────────│            │
│  │ - m_running     │                    │ - handled       │            │
│  │ - m_window      │                    │ - type          │            │
│  │ - m_layerStack  │                    │─────────────────│            │
│  │─────────────────│                    │ + OnEvent()     │            │
│  │ + Run()         │                    │ + GetType()     │            │
│  │ + OnEvent()     │                    │ + ToString()    │            │
│  │ + PushLayer()   │                    └─────────────────┘            │
│  │ + PopLayer()    │                             ▲                     │
│  └─────────────────┘                             │                     │
│           │                                      │                     │
│           │ creates                       ┌─────────────────┐          │
│           ▼                               │ EventDispatcher │          │
│  ┌─────────────────┐                     │─────────────────│          │
│  │     Layer       │                     │ - m_eventQueue  │          │
│  │─────────────────│                     │─────────────────│          │
│  │ + OnAttach()    │                     │ + Dispatch()    │          │
│  │ + OnDetach()    │                     │ + Subscribe()   │          │
│  │ + OnUpdate()    │                     └─────────────────┘          │
│  │ + OnEvent()     │                                                   │
│  │ + OnImGuiRender()│                                                  │
│  └─────────────────┘                                                   │
│           ▲                                                            │
│           │                                                            │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    │
│  │   ImGuiLayer    │    │   SceneLayer    │    │   CustomLayer   │    │
│  │─────────────────│    │─────────────────│    │─────────────────│    │
│  │ + OnImGuiRender()│   │ - m_scene       │    │ + OnUpdate()    │    │
│  │ + Begin()       │    │ - m_camera      │    │ + OnEvent()     │    │
│  │ + End()         │    │─────────────────│    └─────────────────┘    │
│  └─────────────────┘    │ + OnUpdate()    │                           │
│                         │ + OnEvent()     │                           │
│                         │ + OnRender()    │                           │
│                         └─────────────────┘                           │
└─────────────────────────────────────────────────────────────────────────┘
```

## Renderer Architecture

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        Renderer Subsystem                              │
│                                                                         │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    │
│  │    Renderer     │    │   ShaderLib     │    │   MaterialLib   │    │
│  │─────────────────│    │─────────────────│    │─────────────────│    │
│  │ + BeginScene()  │    │ + Add()         │    │ + Load()        │    │
│  │ + EndScene()    │    │ + Get()         │    │ + Get()         │    │
│  │ + Submit()      │    │ + Compile()     │    │ + Create()      │    │
│  │ + SetCamera()   │    └─────────────────┘    └─────────────────┘    │
│  └─────────────────┘                                                   │
│           │                                                            │
│           │ uses                                                       │
│           ▼                                                            │
│  ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    │
│  │     Model       │    │      Mesh       │    │     Texture     │    │
│  │─────────────────│    │─────────────────│    │─────────────────│    │
│  │ - m_meshes      │    │ - vertices      │    │ - m_textureID   │    │
│  │ - m_materials   │    │ - indices       │    │ - m_width       │    │
│  │ - m_model_mat   │    │ - m_VAO         │    │ - m_height      │    │
│  │─────────────────│    │ - m_VBO         │    │─────────────────│    │
│  │ + Load()        │    │ - m_EBO         │    │ + Bind()        │    │
│  │ + Render()      │    │─────────────────│    │ + Load()        │    │
│  │ + Transform()   │    │ + SetupMesh()   │    │ + Create()      │    │
│  └─────────────────┘    │ + Draw()        │    └─────────────────┘    │
│                         └─────────────────┘                           │
└─────────────────────────────────────────────────────────────────────────┘
```

## Sequence Diagram - Frame Rendering

```
┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐
│Application  │  │    Input    │  │    Scene    │  │  Renderer   │  │   Window    │
└─────────────┘  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────┘
      │                │                │                │                │
      │ PollEvents()   │                │                │                │
      │──────────────► │                │                │                │
      │                │                │                │                │
      │                │ OnEvent()      │                │                │
      │                │──────────────► │                │                │
      │                │                │                │                │
      │ Update()       │                │                │                │
      │──────────────────────────────► │                │                │
      │                │                │                │                │
      │                │                │ BeginScene()   │                │
      │                │                │──────────────► │                │
      │                │                │                │                │
      │                │                │ Submit(model)  │                │
      │                │                │──────────────► │                │
      │                │                │                │                │
      │                │                │                │ Draw()         │
      │                │                │                │──────────────► │
      │                │                │                │                │
      │                │                │ EndScene()     │                │
      │                │                │──────────────► │                │
      │                │                │                │                │
      │                │                │                │ SwapBuffers()  │
      │                │                │                │──────────────► │
      │                │                │                │                │
```

## Memory Architecture

ToyEngine uses a combination of standard C++ memory management with smart pointers and custom memory pools:

- **Smart Pointers**: `std::shared_ptr` and `std::unique_ptr` for automatic memory management
- **Custom Allocators**: Planned for performance-critical systems
- **Resource Pools**: Texture and model caching to reduce memory fragmentation
- **RAII Patterns**: Resource acquisition is initialization throughout the engine

## Thread Safety

Currently, ToyEngine is designed as a single-threaded engine with plans for multi-threading:

- **Main Thread**: Handles rendering, input, and game logic
- **Future Work**: Asset loading thread, audio thread, and parallel rendering

## Performance Considerations

- **Frustum Culling**: Planned for scene graph optimization
- **Batch Rendering**: Grouped draw calls for similar objects
- **Shader Caching**: Compiled shaders are cached for reuse
- **Resource Streaming**: Planned for large asset management

## Next Steps

1. Implement scene graph with spatial partitioning
2. Add multi-threading support
3. Implement physically based rendering (PBR)
4. Add audio system integration
5. Implement entity-component-system (ECS) architecture