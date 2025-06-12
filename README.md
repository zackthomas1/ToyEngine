# Toy Engine

## Level 1: System Context
```mermaid
graph TD
    developer[Developer] --> |"builds apps with"| ToyEngine
    ToyEngine --> GLFW[(GLFW)]
    ToyEngine --> OpenGL[(OpenGL)]
```
## Level 2: Container Diagram
```mermaid
graph TD
    app["Application"] --> window["WindowsWindow"]
    app --> renderer["Renderer"]
    app --> scene["Scene"]
    app --> layerMgr["LayerManager"]
    app --> gui["GuiRenderer"]

    window -- input events --> app
    renderer --> camera["Camera"]
    renderer --> material["Material"]
    renderer --> mesh["Mesh"]
    scene -- contains --> model["Model"]
    layerMgr -- manages --> gui & layers["Layers"]
```

## Level 3: Component Diagram (Renderer)

```mermaid
graph TD
    Renderer --> RenderAPI["RenderAPI"]
    Renderer --> Scene
    Renderer --> GuiRenderer
    Scene --> Model
    Scene --> Camera
    Model --> Mesh
    Model --> Material
```
