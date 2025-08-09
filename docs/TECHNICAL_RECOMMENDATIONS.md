# ToyEngine Technical Recommendations

## Detailed Implementation Guidelines

This document provides specific technical recommendations for improving ToyEngine's architecture, including code examples and migration strategies.

## 1. Dependency Injection Implementation

### Current Problem
```cpp
// Current singleton pattern in Application.h
class Application {
    static Application* s_instance;
    static Application& Get() { return *s_instance; }
};

// Usage creates hidden dependencies
void SomeFunction() {
    Application::Get().GetWindow(); // Hidden dependency!
}
```

### Recommended Solution
```cpp
// New dependency injection approach
class ServiceContainer {
private:
    std::unordered_map<std::type_index, std::unique_ptr<void, void(*)(void*)>> services_;
    
public:
    template<typename T, typename... Args>
    void RegisterService(Args&&... args) {
        auto deleter = [](void* obj) { delete static_cast<T*>(obj); };
        services_[std::type_index(typeid(T))] = 
            std::unique_ptr<void, void(*)(void*)>(
                new T(std::forward<Args>(args)...), deleter);
    }
    
    template<typename T>
    T& GetService() {
        auto it = services_.find(std::type_index(typeid(T)));
        if (it != services_.end()) {
            return *static_cast<T*>(it->second.get());
        }
        throw std::runtime_error("Service not registered");
    }
};

// New Application design
class Application {
private:
    ServiceContainer services_;
    
public:
    Application() {
        services_.RegisterService<Window>();
        services_.RegisterService<Renderer>();
        services_.RegisterService<SceneManager>();
    }
    
    template<typename T>
    T& GetService() { return services_.GetService<T>(); }
};
```

## 2. Entity-Component-System Implementation

### Current Problem
```cpp
// Current inheritance-based approach
class Entity {
public:
    virtual void Update(const TimeStep& time_step) {}
    virtual void Render(const glm::mat4& world_transform) const {}
};

class RenderableEntity : public Entity {
    // Specific rendering code
};

class PhysicsEntity : public Entity {
    // Physics code  
};

// What about entities that need both? Diamond inheritance!
```

### Recommended ECS Solution
```cpp
// 1. Component definitions
struct TransformComponent {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
    glm::mat4 worldMatrix{1.0f};
    bool isDirty = true;
};

struct RenderComponent {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    bool visible = true;
};

struct PhysicsComponent {
    glm::vec3 velocity{0.0f};
    float mass = 1.0f;
    bool isStatic = false;
};

// 2. Entity ID system
using EntityID = uint32_t;
constexpr EntityID NULL_ENTITY = 0;

class EntityManager {
private:
    EntityID nextId_ = 1;
    std::queue<EntityID> freeIds_;
    
public:
    EntityID CreateEntity() {
        if (!freeIds_.empty()) {
            EntityID id = freeIds_.front();
            freeIds_.pop();
            return id;
        }
        return nextId_++;
    }
    
    void DestroyEntity(EntityID entity) {
        freeIds_.push(entity);
        // Remove all components for this entity
    }
};

// 3. Component storage
template<typename T>
class ComponentArray {
private:
    std::array<T, MAX_ENTITIES> componentArray_;
    std::unordered_map<EntityID, size_t> entityToIndex_;
    std::unordered_map<size_t, EntityID> indexToEntity_;
    size_t size_ = 0;
    
public:
    void InsertData(EntityID entity, T component) {
        size_t newIndex = size_;
        entityToIndex_[entity] = newIndex;
        indexToEntity_[newIndex] = entity;
        componentArray_[newIndex] = component;
        ++size_;
    }
    
    void RemoveData(EntityID entity) {
        size_t indexOfRemovedEntity = entityToIndex_[entity];
        size_t indexOfLastElement = size_ - 1;
        componentArray_[indexOfRemovedEntity] = componentArray_[indexOfLastElement];
        
        EntityID entityOfLastElement = indexToEntity_[indexOfLastElement];
        entityToIndex_[entityOfLastElement] = indexOfRemovedEntity;
        indexToEntity_[indexOfRemovedEntity] = entityOfLastElement;
        
        entityToIndex_.erase(entity);
        indexToEntity_.erase(indexOfLastElement);
        --size_;
    }
    
    T& GetData(EntityID entity) {
        return componentArray_[entityToIndex_[entity]];
    }
};

// 4. System base class
class System {
public:
    virtual ~System() = default;
    virtual void Update(float deltaTime) = 0;
    
protected:
    std::set<EntityID> entities_;
};

// 5. Specific system implementations
class TransformSystem : public System {
public:
    void Update(float deltaTime) override {
        for (EntityID entity : entities_) {
            auto& transform = ComponentManager::GetComponent<TransformComponent>(entity);
            if (transform.isDirty) {
                UpdateWorldMatrix(transform);
                transform.isDirty = false;
            }
        }
    }
    
private:
    void UpdateWorldMatrix(TransformComponent& transform) {
        transform.worldMatrix = glm::mat4(1.0f);
        transform.worldMatrix = glm::translate(transform.worldMatrix, transform.position);
        // Apply rotation and scale...
    }
};

class RenderSystem : public System {
public:
    void Update(float deltaTime) override {
        for (EntityID entity : entities_) {
            auto& transform = ComponentManager::GetComponent<TransformComponent>(entity);
            auto& render = ComponentManager::GetComponent<RenderComponent>(entity);
            
            if (render.visible && render.mesh && render.material) {
                Renderer::Submit(render.mesh, render.material, transform.worldMatrix);
            }
        }
    }
};
```

## 3. Resource Management System

### Current Problem
```cpp
// Current manual loading approach
auto texture = Texture2D::Create("assets/texture.png");
auto model = Model::LoadFromFile("assets/model.obj");
// No caching, no automatic cleanup
```

### Recommended Resource Manager
```cpp
// 1. Resource base class
class Resource {
public:
    virtual ~Resource() = default;
    virtual bool Load(const std::string& filepath) = 0;
    virtual void Unload() = 0;
    
    const std::string& GetFilepath() const { return filepath_; }
    size_t GetReferenceCount() const { return referenceCount_; }
    
private:
    std::string filepath_;
    mutable std::atomic<size_t> referenceCount_{0};
    friend class ResourceManager;
};

// 2. Specific resource types
class TextureResource : public Resource {
private:
    uint32_t textureId_ = 0;
    int width_, height_, channels_;
    
public:
    bool Load(const std::string& filepath) override {
        // Use stb_image to load texture
        unsigned char* data = stbi_load(filepath.c_str(), &width_, &height_, &channels_, 0);
        if (!data) return false;
        
        glGenTextures(1, &textureId_);
        glBindTexture(GL_TEXTURE_2D, textureId_);
        // Configure texture...
        
        stbi_image_free(data);
        return true;
    }
    
    void Unload() override {
        if (textureId_ != 0) {
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }
    
    uint32_t GetTextureId() const { return textureId_; }
};

// 3. Resource handle system
template<typename T>
class ResourceHandle {
private:
    std::shared_ptr<T> resource_;
    
public:
    ResourceHandle() = default;
    explicit ResourceHandle(std::shared_ptr<T> resource) : resource_(resource) {}
    
    T* operator->() const { return resource_.get(); }
    T& operator*() const { return *resource_; }
    bool IsValid() const { return resource_ != nullptr; }
    
    operator bool() const { return IsValid(); }
};

// 4. Resource manager
class ResourceManager {
private:
    std::unordered_map<std::string, std::weak_ptr<Resource>> loadedResources_;
    mutable std::shared_mutex resourceMutex_;
    
public:
    template<typename T>
    ResourceHandle<T> Load(const std::string& filepath) {
        std::unique_lock lock(resourceMutex_);
        
        // Check if already loaded
        auto it = loadedResources_.find(filepath);
        if (it != loadedResources_.end()) {
            if (auto existing = it->second.lock()) {
                return ResourceHandle<T>(std::static_pointer_cast<T>(existing));
            }
        }
        
        // Load new resource
        auto resource = std::make_shared<T>();
        if (resource->Load(filepath)) {
            loadedResources_[filepath] = resource;
            return ResourceHandle<T>(resource);
        }
        
        return ResourceHandle<T>();
    }
    
    void UnloadUnused() {
        std::unique_lock lock(resourceMutex_);
        
        auto it = loadedResources_.begin();
        while (it != loadedResources_.end()) {
            if (it->second.expired()) {
                it = loadedResources_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    size_t GetLoadedResourceCount() const {
        std::shared_lock lock(resourceMutex_);
        return loadedResources_.size();
    }
};

// 5. Usage example
void LoadGameAssets() {
    auto& resourceManager = ServiceContainer::GetService<ResourceManager>();
    
    auto playerTexture = resourceManager.Load<TextureResource>("assets/player.png");
    auto enemyModel = resourceManager.Load<ModelResource>("assets/enemy.obj");
    
    // Resources are automatically cached and reference-counted
    auto playerTexture2 = resourceManager.Load<TextureResource>("assets/player.png");
    // Same instance returned, no duplicate loading
}
```

## 4. Multi-threading Architecture

### Current Problem
```cpp
// Current single-threaded update loop
void Application::Run() {
    while (isRunning_) {
        // Everything runs on main thread
        UpdateInput();
        UpdateSystems();
        Render();
        Present();
    }
}
```

### Recommended Multi-threaded Solution
```cpp
// 1. Job System
class Job {
public:
    virtual ~Job() = default;
    virtual void Execute() = 0;
};

class JobSystem {
private:
    std::vector<std::thread> workers_;
    std::queue<std::unique_ptr<Job>> jobQueue_;
    std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> shouldStop_{false};
    
public:
    JobSystem(size_t numThreads = std::thread::hardware_concurrency()) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back([this] { WorkerLoop(); });
        }
    }
    
    ~JobSystem() {
        shouldStop_ = true;
        condition_.notify_all();
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    void AddJob(std::unique_ptr<Job> job) {
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            jobQueue_.push(std::move(job));
        }
        condition_.notify_one();
    }
    
    template<typename F>
    void AddJob(F&& func) {
        struct FunctionJob : public Job {
            F function;
            explicit FunctionJob(F&& f) : function(std::forward<F>(f)) {}
            void Execute() override { function(); }
        };
        AddJob(std::make_unique<FunctionJob>(std::forward<F>(func)));
    }
    
private:
    void WorkerLoop() {
        while (!shouldStop_) {
            std::unique_ptr<Job> job;
            {
                std::unique_lock<std::mutex> lock(queueMutex_);
                condition_.wait(lock, [this] { return !jobQueue_.empty() || shouldStop_; });
                
                if (shouldStop_) break;
                
                job = std::move(jobQueue_.front());
                jobQueue_.pop();
            }
            
            if (job) {
                job->Execute();
            }
        }
    }
};

// 2. Thread-safe command buffer for rendering
class RenderCommand {
public:
    virtual ~RenderCommand() = default;
    virtual void Execute() = 0;
};

class RenderCommandBuffer {
private:
    std::vector<std::unique_ptr<RenderCommand>> commands_;
    mutable std::mutex commandMutex_;
    
public:
    template<typename T, typename... Args>
    void AddCommand(Args&&... args) {
        std::lock_guard<std::mutex> lock(commandMutex_);
        commands_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }
    
    void ExecuteAll() {
        std::lock_guard<std::mutex> lock(commandMutex_);
        for (auto& command : commands_) {
            command->Execute();
        }
        commands_.clear();
    }
};

// 3. Multi-threaded application loop
class ThreadedApplication : public Application {
private:
    std::unique_ptr<JobSystem> jobSystem_;
    RenderCommandBuffer renderCommands_;
    std::thread renderThread_;
    std::atomic<bool> shouldRender_{true};
    
public:
    void Run() override {
        jobSystem_ = std::make_unique<JobSystem>();
        renderThread_ = std::thread([this] { RenderLoop(); });
        
        while (isRunning_) {
            // Main thread handles input and game logic
            UpdateInput();
            
            // Submit system updates to job system
            jobSystem_->AddJob([this] { UpdatePhysicsSystem(); });
            jobSystem_->AddJob([this] { UpdateAudioSystem(); });
            jobSystem_->AddJob([this] { UpdateAISystem(); });
            
            // Update render system (adds commands to buffer)
            UpdateRenderSystem();
            
            // Brief sleep to prevent busy waiting
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        
        shouldRender_ = false;
        if (renderThread_.joinable()) {
            renderThread_.join();
        }
    }
    
private:
    void RenderLoop() {
        while (shouldRender_) {
            renderCommands_.ExecuteAll();
            Present();
            
            // Target 60 FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
};
```

## 5. Platform Abstraction Layer

### Current Problem
```cpp
// Platform-specific code scattered throughout
#ifdef TY_PLATFORM_WINDOWS
    // Windows-specific implementation
#else
    #error Only supports Windows
#endif
```

### Recommended HAL Implementation
```cpp
// 1. Platform interface definitions
class IPlatformWindow {
public:
    virtual ~IPlatformWindow() = default;
    virtual bool Create(int width, int height, const std::string& title) = 0;
    virtual void Destroy() = 0;
    virtual void SwapBuffers() = 0;
    virtual bool ShouldClose() const = 0;
    virtual void PollEvents() = 0;
};

class IPlatformInput {
public:
    virtual ~IPlatformInput() = default;
    virtual bool IsKeyPressed(int keycode) const = 0;
    virtual bool IsMouseButtonPressed(int button) const = 0;
    virtual std::pair<float, float> GetMousePosition() const = 0;
};

class IPlatformFileSystem {
public:
    virtual ~IPlatformFileSystem() = default;
    virtual bool FileExists(const std::string& path) const = 0;
    virtual std::vector<uint8_t> ReadFile(const std::string& path) const = 0;
    virtual bool WriteFile(const std::string& path, const std::vector<uint8_t>& data) const = 0;
    virtual std::vector<std::string> ListDirectory(const std::string& path) const = 0;
};

// 2. Platform factory
class PlatformFactory {
public:
    static std::unique_ptr<IPlatformWindow> CreateWindow();
    static std::unique_ptr<IPlatformInput> CreateInput();
    static std::unique_ptr<IPlatformFileSystem> CreateFileSystem();
};

// 3. Windows implementation
#ifdef TY_PLATFORM_WINDOWS
class WindowsWindow : public IPlatformWindow {
private:
    GLFWwindow* window_ = nullptr;
    
public:
    bool Create(int width, int height, const std::string& title) override {
        if (!glfwInit()) return false;
        
        window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window_) {
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(window_);
        return true;
    }
    
    void Destroy() override {
        if (window_) {
            glfwDestroyWindow(window_);
            window_ = nullptr;
        }
        glfwTerminate();
    }
    
    // Other implementations...
};

std::unique_ptr<IPlatformWindow> PlatformFactory::CreateWindow() {
    return std::make_unique<WindowsWindow>();
}
#endif

// 4. Cross-platform usage
class Application {
private:
    std::unique_ptr<IPlatformWindow> window_;
    std::unique_ptr<IPlatformInput> input_;
    std::unique_ptr<IPlatformFileSystem> fileSystem_;
    
public:
    Application() {
        window_ = PlatformFactory::CreateWindow();
        input_ = PlatformFactory::CreateInput();
        fileSystem_ = PlatformFactory::CreateFileSystem();
    }
    
    // Platform-agnostic implementation
};
```

## Migration Strategy

### Phase 1: Foundation Changes (Minimal Disruption)
1. **Add Service Container**: Introduce alongside existing singletons
2. **Create Resource Manager**: Start with texture resources only
3. **Implement Basic ECS**: Begin with Transform and Render components

### Phase 2: System Replacement
1. **Replace Application Singleton**: Migrate to dependency injection
2. **Replace Entity Inheritance**: Convert existing entities to ECS
3. **Add Platform Abstraction**: Create interfaces, implement Windows version

### Phase 3: Advanced Features
1. **Add Multi-threading**: Implement job system and render thread
2. **Expand ECS**: Add physics, audio, and other component types
3. **Platform Expansion**: Implement Linux and macOS support

## Testing Strategy

### Unit Testing Framework
```cpp
// Example unit test for ECS system
class MockComponentManager {
    // Test doubles for component storage
};

TEST(TransformSystemTest, UpdatesWorldMatrix) {
    MockComponentManager components;
    TransformSystem system;
    
    EntityID entity = 1;
    TransformComponent transform;
    transform.position = glm::vec3(1.0f, 2.0f, 3.0f);
    transform.isDirty = true;
    
    components.AddComponent(entity, transform);
    system.AddEntity(entity);
    
    system.Update(0.016f);
    
    auto& updatedTransform = components.GetComponent<TransformComponent>(entity);
    EXPECT_FALSE(updatedTransform.isDirty);
    // Verify world matrix is correct...
}
```

## Performance Considerations

1. **Memory Layout**: Use structure-of-arrays for components to improve cache performance
2. **Component Pools**: Pre-allocate component storage to avoid runtime allocations
3. **System Ordering**: Order systems to minimize cache misses
4. **Job Granularity**: Balance job size to minimize thread overhead
5. **Resource Streaming**: Implement background loading for large assets

These technical recommendations provide a roadmap for transforming ToyEngine into a modern, scalable game engine while maintaining backwards compatibility during the transition.