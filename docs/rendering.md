# Rendering Pipeline Documentation

This document describes ToyEngine's rendering pipeline, graphics systems, and rendering techniques.

## Overview

ToyEngine uses a forward rendering pipeline with OpenGL 4.3+ as the primary graphics API. The renderer is designed to be modular and extensible, supporting multiple rendering techniques and optimization strategies.

## Rendering Architecture

### High-Level Pipeline

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Scene Graph   │───►│   Visibility    │───►│   Render        │
│   Traversal     │    │   Culling       │    │   Submission    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Transform     │    │   Frustum       │    │   Command       │
│   Updates       │    │   Culling       │    │   Buffer        │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                                                        │
                                                        ▼
                                            ┌─────────────────┐
                                            │   GPU           │
                                            │   Execution     │
                                            └─────────────────┘
```

### Rendering Stages

1. **Scene Setup**: Camera configuration and scene graph preparation
2. **Culling**: Frustum culling and occlusion testing
3. **Sorting**: Depth sorting and state grouping
4. **Submission**: Command buffer generation and GPU submission
5. **Execution**: GPU rendering and post-processing

## Core Rendering Components

### Renderer Class

The main rendering interface that orchestrates the entire pipeline.

```cpp
class Renderer
{
public:
    static void BeginScene(const Ref<Camera>& camera);
    static void EndScene();
    static void Submit(const Ref<Shader>& shader, const Ref<Model>& model);
    
    static void SetClearColor(const glm::vec4& color);
    static void Clear();
    
    static void EnableDepthTest(bool enable);
    static void EnableBlending(bool enable);
    static void EnableFaceCulling(bool enable);
    
    static UniformBufferManager& GetUniformManager();
    
private:
    static void Initialize();
    static void Shutdown();
    
    static Ref<UniformBufferManager> s_UniformManager;
    static Ref<Camera> s_Camera;
    static RenderStats s_Stats;
};
```

### Shader Management

#### Shader Class
Handles GLSL shader compilation, linking, and uniform management.

```cpp
class Shader
{
public:
    static Ref<Shader> Create(const std::string& name, 
                              const std::string& vertexPath, 
                              const std::string& fragmentPath);
    
    void Use() const;
    void Unuse() const;
    
    // Uniform setters
    void SetFloat(const std::string& name, float value);
    void SetFloat2(const std::string& name, const glm::vec2& value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetFloat4(const std::string& name, const glm::vec4& value);
    void SetInt(const std::string& name, int value);
    void SetBool(const std::string& name, bool value);
    void SetMat4(const std::string& name, const glm::mat4& matrix);
    
    // Uniform block binding
    void BindUniformBlock(const std::string& name, uint32_t binding);
    
private:
    uint32_t CompileShader(const std::string& source, GLenum type);
    void LinkProgram();
    int GetUniformLocation(const std::string& name) const;
    
    uint32_t m_RendererID;
    std::string m_Name;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
```

#### Shader Library
Manages shader compilation and caching.

```cpp
class ShaderLibrary
{
public:
    void Add(const Ref<Shader>& shader);
    void Load(const std::string& name, const std::string& filepath);
    void LoadFromSource(const std::string& name, const std::string& source);
    
    Ref<Shader> Get(const std::string& name);
    bool Exists(const std::string& name) const;
    
private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};
```

### Mesh and Model System

#### Vertex Data Structure
```cpp
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};
```

#### Mesh Class
Handles individual mesh rendering.

```cpp
class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Ref<Texture>> textures;
    
    Mesh(const std::vector<Vertex>& vertices, 
         const std::vector<uint32_t>& indices,
         const std::vector<Ref<Texture>>& textures);
    
    void Draw(const Ref<Shader>& shader);
    
private:
    uint32_t m_VAO, m_VBO, m_EBO;
    void SetupMesh();
};
```

#### Model Class
Manages collections of meshes and materials.

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
    std::vector<Ref<Texture>> LoadMaterialTextures(aiMaterial* mat, 
                                                   aiTextureType type,
                                                   const std::string& typeName);
};
```

### Texture Management

#### Texture Class
```cpp
class Texture
{
public:
    static Ref<Texture> Create(const std::string& path);
    static Ref<Texture> Create(uint32_t width, uint32_t height, 
                               const void* data, GLenum format);
    
    void Bind(uint32_t slot = 0) const;
    void Unbind() const;
    
    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }
    uint32_t GetRendererID() const { return m_RendererID; }
    
private:
    uint32_t m_RendererID;
    uint32_t m_Width, m_Height;
    GLenum m_InternalFormat, m_DataFormat;
    std::string m_Path;
};
```

### Material System

#### Material Class
```cpp
class Material
{
public:
    Material(const std::string& name);
    
    void SetTexture(const std::string& name, const Ref<Texture>& texture);
    void SetFloat(const std::string& name, float value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetFloat4(const std::string& name, const glm::vec4& value);
    
    void Bind(const Ref<Shader>& shader) const;
    
private:
    std::string m_Name;
    std::unordered_map<std::string, Ref<Texture>> m_Textures;
    std::unordered_map<std::string, float> m_FloatProperties;
    std::unordered_map<std::string, glm::vec3> m_Vec3Properties;
    std::unordered_map<std::string, glm::vec4> m_Vec4Properties;
};
```

## Lighting System

### Light Types

#### Light Class
```cpp
enum class eLightType
{
    kDirectional,
    kPoint,
    kSpot
};

class Light
{
public:
    Light(eLightType type);
    
    eLightType m_type;
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_value = glm::vec3(1.0f);
    
    // Spot light parameters
    glm::vec3 m_spotDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    float m_innerAngle = 12.5f;
    float m_outerAngle = 17.5f;
    
    // Attenuation parameters
    float m_constant = 1.0f;
    float m_linear = 0.09f;
    float m_quadratic = 0.032f;
    
    bool m_enabled = true;
    
    void SetUniforms(const Ref<Shader>& shader, const std::string& baseName) const;
};
```

### Lighting Calculations

#### Phong Lighting Model
The current implementation uses the Phong lighting model with support for multiple light types.

**Vertex Shader (lighting.vs):**
```glsl
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std140, binding = 0) uniform ViewProjectMats
{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;
uniform mat3 normalMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalMatrix * aNormal;
    TexCoord = aTexCoord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

**Fragment Shader (lighting.fs):**
```glsl
#version 430 core

struct DirectionalLight
{
    vec3 direction;
    vec3 value;
    bool enabled;
};

struct PointLight
{
    vec3 position;
    vec3 value;
    bool enabled;
    
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 value;
    bool enabled;
    
    float innerAngle;
    float outerAngle;
    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

#define MAX_LIGHTS 8

uniform DirectionalLight uDirectionalLights[MAX_LIGHTS];
uniform PointLight uPointLights[MAX_LIGHTS];
uniform SpotLight uSpotLights[MAX_LIGHTS];
uniform int uNumDirectionalLights;
uniform int uNumPointLights;
uniform int uNumSpotLights;

uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0);
    
    // Directional lights
    for(int i = 0; i < uNumDirectionalLights && i < MAX_LIGHTS; i++)
    {
        if(uDirectionalLights[i].enabled)
            result += CalcDirectionalLight(uDirectionalLights[i], norm, viewDir);
    }
    
    // Point lights
    for(int i = 0; i < uNumPointLights && i < MAX_LIGHTS; i++)
    {
        if(uPointLights[i].enabled)
            result += CalcPointLight(uPointLights[i], norm, FragPos, viewDir);
    }
    
    // Spot lights
    for(int i = 0; i < uNumSpotLights && i < MAX_LIGHTS; i++)
    {
        if(uSpotLights[i].enabled)
            result += CalcSpotLight(uSpotLights[i], norm, FragPos, viewDir);
    }
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Combine results
    vec3 ambient = 0.1 * light.value * texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = diff * light.value * texture(material.diffuse, TexCoord).rgb;
    vec3 specular = spec * light.value * texture(material.specular, TexCoord).rgb;
    
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Combine results
    vec3 ambient = 0.1 * light.value * texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = diff * light.value * texture(material.diffuse, TexCoord).rgb;
    vec3 specular = spec * light.value * texture(material.specular, TexCoord).rgb;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = cos(radians(light.innerAngle)) - cos(radians(light.outerAngle));
    float intensity = clamp((theta - cos(radians(light.outerAngle))) / epsilon, 0.0, 1.0);
    
    // Combine results
    vec3 ambient = 0.1 * light.value * texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = diff * light.value * texture(material.diffuse, TexCoord).rgb;
    vec3 specular = spec * light.value * texture(material.specular, TexCoord).rgb;
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ambient + diffuse + specular;
}
```

## Camera System

### Camera Class
Handles view and projection matrix calculations.

```cpp
enum class eCameraType
{
    kFlyCamera,
    kOrbitCamera
};

enum class eCameraMovement
{
    kForward,
    kBackward,
    kLeft,
    kRight,
    kUp,
    kDown
};

class Camera
{
public:
    Camera(eCameraType type = eCameraType::kFlyCamera);
    
    void UpdatePosition(eCameraMovement direction, float deltaTime);
    void UpdateLookDirection(float xoffset, float yoffset, bool constrainPitch = true);
    void UpdateFOV(float yoffset);
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspect) const;
    
    // Getters
    const glm::vec3& position() const { return m_position; }
    const glm::vec3& front() const { return m_front; }
    const glm::vec3& up() const { return m_up; }
    const glm::vec3& right() const { return m_right; }
    float fov() const { return m_fov; }
    
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

## Uniform Buffer Management

### Uniform Buffer Objects (UBOs)
For efficient uniform data management across multiple shaders.

```cpp
class UniformBufferManager
{
public:
    UniformBufferManager();
    ~UniformBufferManager();
    
    void CreateBuffer(const std::string& name, uint32_t size, uint32_t binding);
    void UpdateBuffer(const std::string& name, const void* data, uint32_t size, uint32_t offset = 0);
    uint32_t GetBindPoint(const std::string& name) const;
    
private:
    struct UniformBuffer
    {
        uint32_t ID;
        uint32_t binding;
        uint32_t size;
    };
    
    std::unordered_map<std::string, UniformBuffer> m_buffers;
    uint32_t m_nextBinding = 0;
};
```

## Render Statistics

### Performance Monitoring
```cpp
struct RenderStats
{
    uint32_t DrawCalls = 0;
    uint32_t VertexCount = 0;
    uint32_t IndexCount = 0;
    uint32_t TextureBinds = 0;
    uint32_t ShaderBinds = 0;
    float FrameTime = 0.0f;
    
    void Reset()
    {
        DrawCalls = 0;
        VertexCount = 0;
        IndexCount = 0;
        TextureBinds = 0;
        ShaderBinds = 0;
        FrameTime = 0.0f;
    }
};
```

## Future Enhancements

### Planned Features

1. **Deferred Rendering**: For handling many lights efficiently
2. **Shadow Mapping**: Dynamic shadow casting
3. **Physically Based Rendering (PBR)**: More realistic materials
4. **Post-Processing Pipeline**: Bloom, tone mapping, etc.
5. **Instanced Rendering**: Efficient rendering of multiple objects
6. **Compute Shaders**: GPU-accelerated calculations
7. **Texture Streaming**: Dynamic texture loading
8. **Level of Detail (LOD)**: Distance-based model simplification

### Optimization Strategies

1. **Frustum Culling**: Only render visible objects
2. **Occlusion Culling**: Skip objects hidden by others
3. **Batch Rendering**: Group similar draw calls
4. **State Sorting**: Minimize state changes
5. **Texture Atlasing**: Combine multiple textures
6. **Shader Variants**: Specialized shaders for different scenarios

## Best Practices

### Performance Tips

1. **Minimize State Changes**: Group objects by material and shader
2. **Use Appropriate Data Types**: Choose efficient vertex formats
3. **Optimize Shaders**: Keep fragment shaders simple
4. **Profile Regularly**: Use graphics debuggers and profilers
5. **Test on Target Hardware**: Verify performance on intended devices

### Code Organization

1. **Separate Concerns**: Keep rendering logic separate from game logic
2. **Use Abstractions**: Hide platform-specific details
3. **Document Shaders**: Comment complex shader code
4. **Version Control**: Track shader and asset changes
5. **Automated Testing**: Test rendering correctness automatically

This rendering pipeline provides a solid foundation for 3D graphics while remaining extensible for future enhancements and optimizations.