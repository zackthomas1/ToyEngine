# Core API Reference

This document describes the core classes and functions in ToyEngine.

## Application Class

The main application class that manages the engine lifecycle.

### Declaration
```cpp
class Application
{
public:
    Application();
    virtual ~Application();
    
    void Run();
    void OnEvent(Event& e);
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    
    static Application& Get() { return *s_Instance; }
    Window& GetWindow() { return *m_Window; }
    
private:
    bool OnWindowClose(WindowCloseEvent& e);
    
    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;
    bool m_Running = true;
    
    static Application* s_Instance;
};
```

### Usage Example
```cpp
class MyApp : public ToyEngine::Application
{
public:
    MyApp()
    {
        PushLayer(new MyLayer());
    }
};

// In your main function
ToyEngine::Application* ToyEngine::CreateApplication()
{
    return new MyApp();
}
```

## Layer System

### Layer Class
Base class for all engine layers.

```cpp
class Layer
{
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();
    
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(const TimeStep& timeStep) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) {}
    
    const std::string& GetName() const { return m_DebugName; }
    
protected:
    std::string m_DebugName;
};
```

### LayerStack Class
Manages the collection of layers.

```cpp
class LayerStack
{
public:
    LayerStack();
    ~LayerStack();
    
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);
    
    std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
    std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    
private:
    std::vector<Layer*> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};
```

## Event System

### Event Base Class
```cpp
class Event
{
public:
    bool Handled = false;
    
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }
    
    inline bool IsInCategory(EventCategory category)
    {
        return GetCategoryFlags() & category;
    }
    
    template<typename T>
    bool IsEventType()
    {
        return GetEventType() == T::GetStaticType();
    }
};
```

### Common Event Types
```cpp
// Window Events
class WindowCloseEvent : public Event { /* ... */ };
class WindowResizeEvent : public Event { /* ... */ };

// Key Events
class KeyPressedEvent : public Event { /* ... */ };
class KeyReleasedEvent : public Event { /* ... */ };

// Mouse Events
class MouseButtonPressedEvent : public Event { /* ... */ };
class MouseButtonReleasedEvent : public Event { /* ... */ };
class MouseMovedEvent : public Event { /* ... */ };
class MouseScrolledEvent : public Event { /* ... */ };
```

## Renderer System

### Renderer Class
Main rendering interface.

```cpp
class Renderer
{
public:
    static void BeginScene(const Ref<Camera>& camera);
    static void EndScene();
    static void Submit(const Ref<Shader>& shader, const Ref<Model>& model);
    
    static UniformBufferManager& GetUniformManager() { return *s_UniformManager; }
    
private:
    static Ref<UniformBufferManager> s_UniformManager;
    static Ref<Camera> s_Camera;
};
```

### Shader Class
Manages OpenGL shaders.

```cpp
class Shader
{
public:
    static Ref<Shader> Create(const std::string& name, 
                              const std::string& vertexPath, 
                              const std::string& fragmentPath);
    
    void Use() const;
    void SetFloat(const std::string& name, float value);
    void SetFloat3(const std::string& name, float x, float y, float z);
    void SetFloat4(const std::string& name, float x, float y, float z, float w);
    void SetInt(const std::string& name, int value);
    void SetBool(const std::string& name, bool value);
    void SetMat4(const std::string& name, const glm::mat4& matrix);
    
    void BindUniformBlock(const std::string& name, uint32_t binding);
    
    const std::string& GetName() const { return m_Name; }
    
private:
    std::string m_Name;
    uint32_t m_RendererID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
```

### Model Class
Represents 3D models with meshes and materials.

```cpp
class Model
{
public:
    static Ref<Model> Create(const std::string& path, bool flipUVs = false);
    
    void Draw(const Ref<Shader>& shader);
    
    glm::mat4 m_model_mat = glm::mat4(1.0f);
    
private:
    std::vector<Ref<Mesh>> m_meshes;
    std::vector<Ref<Material>> m_materials;
    std::string m_directory;
    
    void LoadModel(const std::string& path, bool flipUVs);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
```

## Input System

### InputPoll Class
Handles input polling and state management.

```cpp
class InputPoll
{
public:
    virtual eKeyState Key(eKeyCode key) = 0;
    virtual eKeyState MouseButton(eMouseButton button) = 0;
    virtual std::pair<double, double> MousePosition() = 0;
    virtual double MouseX() = 0;
    virtual double MouseY() = 0;
    
    static InputPoll& Get() { return *s_Instance; }
    
private:
    static InputPoll* s_Instance;
};
```

### Key Codes
```cpp
enum class eKeyCode
{
    kKeyA = 65, kKeyB = 66, kKeyC = 67, kKeyD = 68,
    kKeyE = 69, kKeyF = 70, kKeyG = 71, kKeyH = 72,
    // ... more keys
    kKeyW = 87, kKeyX = 88, kKeyY = 89, kKeyZ = 90,
    
    kKeySpace = 32,
    kKeyEscape = 256,
    kKeyEnter = 257,
    kKeyTab = 258,
    kKeyBackspace = 259,
    // ... more keys
};

enum class eKeyState
{
    kRelease = 0,
    kPress = 1,
    kRepeat = 2
};
```

## Camera System

### Camera Class
Handles view and projection matrices.

```cpp
class Camera
{
public:
    Camera(eCameraType type = eCameraType::kFlyCamera);
    
    void UpdatePosition(eCameraMovement direction, float deltaTime);
    void UpdateLookDirection(float xoffset, float yoffset, bool constrainPitch = true);
    void UpdateFOV(float yoffset);
    
    const glm::vec3& position() const { return m_position; }
    const glm::vec3& front() const { return m_front; }
    float fov() const { return m_fov; }
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspect) const;
    
private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    
    float m_yaw;
    float m_pitch;
    float m_fov;
    float m_movementSpeed;
    float m_mouseSensitivity;
    
    eCameraType m_type;
    
    void UpdateCameraVectors();
};
```

## Utility Classes

### TimeStep Class
Manages frame timing.

```cpp
class TimeStep
{
public:
    TimeStep(float time = 0.0f) : m_time(time) {}
    
    float GetSeconds() const { return m_time; }
    float GetMilliseconds() const { return m_time * 1000.0f; }
    float GetTimeDelta() const { return m_time; }
    
    operator float() const { return m_time; }
    
private:
    float m_time;
};
```

### Service Locator
Provides global access to engine services.

```cpp
class Locator
{
public:
    static InputPoll& InputPollService();
    static void Provide(InputPoll* service);
    
private:
    static InputPoll* s_inputPollService;
};
```

## Memory Management

### Smart Pointer Aliases
```cpp
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr Scope<T> MakeScope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}
```

## Logging System

### Log Macros
```cpp
// Core logging (for engine internals)
#define TY_CORE_TRACE(...)    ::ToyEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TY_CORE_INFO(...)     ::ToyEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TY_CORE_WARN(...)     ::ToyEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TY_CORE_ERROR(...)    ::ToyEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TY_CORE_CRITICAL(...) ::ToyEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client logging (for applications)
#define TY_TRACE(...)         ::ToyEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TY_INFO(...)          ::ToyEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define TY_WARN(...)          ::ToyEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TY_ERROR(...)         ::ToyEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define TY_CRITICAL(...)      ::ToyEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
```

## Usage Examples

### Creating a Simple Application
```cpp
#include <toy_engine.h>

class MyLayer : public ToyEngine::Layer
{
public:
    MyLayer() : Layer("MyLayer") {}
    
    void OnUpdate(const ToyEngine::TimeStep& timeStep) override
    {
        // Update logic here
    }
    
    void OnImGuiRender() override
    {
        ImGui::Begin("My Window");
        ImGui::Text("Hello, World!");
        ImGui::End();
    }
    
    void OnEvent(ToyEngine::Event& event) override
    {
        if (event.GetEventType() == ToyEngine::EventType::KeyPressed)
        {
            TY_INFO("Key pressed!");
        }
    }
};

class MyApp : public ToyEngine::Application
{
public:
    MyApp()
    {
        PushLayer(new MyLayer());
    }
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
    return new MyApp();
}
```

### Loading and Rendering a Model
```cpp
void OnAttach() override
{
    // Load shader
    auto shader = ToyEngine::Shader::Create("basic", 
        "assets/shaders/basic.vs", 
        "assets/shaders/basic.fs");
    
    // Load model
    m_model = ToyEngine::Model::Create("assets/models/cube.obj");
    
    // Create camera
    m_camera = ToyEngine::MakeRef<ToyEngine::Camera>();
}

void OnUpdate(const ToyEngine::TimeStep& timeStep) override
{
    // Handle input
    ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();
    if (input.Key(ToyEngine::eKeyCode::kKeyW) != ToyEngine::eKeyState::kRelease)
        m_camera->UpdatePosition(ToyEngine::eCameraMovement::kForward, timeStep);
    
    // Render
    ToyEngine::Renderer::BeginScene(m_camera);
    ToyEngine::Renderer::Submit(shader, m_model);
    ToyEngine::Renderer::EndScene();
}
```

For more detailed examples and tutorials, see the [Tutorials](../tutorials/) section.