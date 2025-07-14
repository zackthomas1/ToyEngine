# ToyEngine Entity Component System (ECS) Documentation

## Overview

The ToyEngine ECS system provides a flexible, component-based architecture for managing game entities and their behaviors. It implements a scene graph system where entities contain components that dictate their behavior and properties.

## Core Components

### Component System
- **Component**: Base interface for all components
- **TypedComponent<T>**: Template base class that provides automatic type identification
- Components handle their own lifecycle through `OnAttach()`, `OnDetach()`, and `Update(deltaTime)`

### Entity Management
- **Entity**: Container for components with unique ID and name
- Type-safe component addition, removal, and querying
- Automatic component lifecycle management
- Enable/disable functionality for entities

### Scene Management
- **Scene**: Container for entities providing scene graph functionality
- Entity creation, destruction, and querying
- Parent-child relationships through Transform components
- Scene-wide update and rendering coordination

### Core Components

#### Transform Component
- Position, rotation, and scale in 3D space
- Hierarchical parent-child relationships
- World and local space transformations
- Automatic dirty tracking for performance

#### Renderable Component
- Integration with ToyEngine's existing renderer
- Model and shader assignment
- Visibility and render layer control
- Shadow casting/receiving options

### Scene Rendering
- **SceneRenderer**: Integrates ECS with existing renderer
- Automatic transform updates
- Render layer sorting
- Frustum culling support (extensible)

## Basic Usage

### Creating a Scene and Entities

```cpp
// Create a scene
auto scene = ToyEngine::MakeScope<ToyEngine::Scene>("My Scene");

// Create an entity
ToyEngine::Entity& entity = scene->CreateEntity("My Entity");

// Every entity automatically has a Transform component
ToyEngine::Transform& transform = entity.GetComponent<ToyEngine::Transform>();
transform.SetLocalPosition(glm::vec3(1.0f, 0.0f, 0.0f));
```

### Adding Components

```cpp
// Add a renderable component
ToyEngine::Renderable& renderable = entity.AddComponent<ToyEngine::Renderable>();
renderable.SetModel(my_model);
renderable.SetShader(my_shader);

// Add custom components
entity.AddComponent<MyCustomComponent>(constructor_args);
```

### Component Querying

```cpp
// Check if entity has a component
if (entity.HasComponent<ToyEngine::Renderable>()) {
    // Get component reference
    ToyEngine::Renderable& renderable = entity.GetComponent<ToyEngine::Renderable>();
    
    // Or try to get component (returns pointer or nullptr)
    ToyEngine::Renderable* renderable_ptr = entity.TryGetComponent<ToyEngine::Renderable>();
}
```

### Scene Graph Hierarchies

```cpp
// Create parent and child entities
ToyEngine::Entity& parent = scene->CreateEntity("Parent");
ToyEngine::Entity& child = scene->CreateEntity("Child");

// Set up parent-child relationship
scene->SetEntityParent(parent, child);

// Child transforms are now relative to parent
child.GetComponent<ToyEngine::Transform>().SetLocalPosition(glm::vec3(1.0f, 0.0f, 0.0f));
```

### Scene Rendering

```cpp
// Update scene (updates all entities and components)
scene->Update(deltaTime);

// Render scene
ToyEngine::SceneRenderer::RenderScene(*scene, camera);
```

## Creating Custom Components

### Basic Component

```cpp
class MyComponent : public ToyEngine::TypedComponent<MyComponent>
{
public:
    MyComponent(int value) : value_(value) {}
    
    void Update(float deltaTime) override
    {
        // Component logic here
    }
    
    void OnAttach() override
    {
        // Called when component is added to entity
    }
    
    void OnDetach() override
    {
        // Called when component is removed from entity
    }
    
    int GetValue() const { return value_; }
    void SetValue(int value) { value_ = value; }
    
private:
    int value_;
};
```

### Using Custom Components

```cpp
// Add to entity
MyComponent& comp = entity.AddComponent<MyComponent>(42);

// Use component
comp.SetValue(100);
int value = comp.GetValue();

// Query entities with specific components
auto entities_with_my_component = scene->GetEntitiesWithComponent<MyComponent>();
```

## Scene Querying

```cpp
// Get all entities
auto all_entities = scene->GetAllEntities();

// Get entities with specific components
auto renderable_entities = scene->GetEntitiesWithComponent<ToyEngine::Renderable>();

// Get root entities (no parent)
auto root_entities = scene->GetRootEntities();

// Find entity by name or ID
ToyEngine::Entity* entity = scene->GetEntity("Entity Name");
ToyEngine::Entity* entity = scene->GetEntity(entity_id);
```

## Integration with Existing ToyEngine Systems

The ECS system is designed to work seamlessly with existing ToyEngine components:

- **Renderer**: Renderable components integrate with Model and Shader systems
- **Camera**: Works with existing Camera system for rendering
- **Layers**: ECS scenes can be used within existing Layer architecture
- **Events**: Entities can respond to events through custom components

## Performance Considerations

- Components use dirty tracking to avoid unnecessary recalculations
- Scene rendering sorts entities by render layer for optimal performance
- Type-safe component queries avoid runtime type checking overhead
- Memory is managed through smart pointers for automatic cleanup

## Example: Editor Integration

See `Editor/src/editor_ecs.cpp` for a complete example of integrating the ECS system with ToyEngine's existing editor framework.

## Extending the System

The ECS system is designed to be extensible:

1. Create custom components by inheriting from `TypedComponent<T>`
2. Add component-specific systems by querying scenes for entities with specific components
3. Extend rendering by adding new render layers or custom rendering logic
4. Add new component types for physics, audio, AI, etc.

The system maintains the flexibility of composition over inheritance while providing type safety and performance.