# TextureCube Implementation

This document describes the newly implemented cube mapping texture functionality for creating skyboxes in ToyEngine.

## Overview

The TextureCube class provides a complete implementation for loading and using cube map textures, following the same design patterns as the existing Texture2D class. This enables skybox rendering and other environment mapping effects.

## Classes

### TextureCube (Base Class)
- **Location**: `ToyEngine/src/ToyEngine/renderer/texture_cube.h`
- **Purpose**: Abstract base class defining the cube map texture interface
- **Key Methods**:
  - `Create(const Array<std::string, 6>& face_paths, bool flip_vertically = false)` - Create from 6 face images
  - `Create(const std::string& path, bool flip_vertically = false)` - Create from single file (future)
  - `Bind(uint32_t texture_unit)` - Bind texture to OpenGL texture unit
  - `GetWidth()`, `GetHeight()` - Get texture dimensions

### OpenGLTextureCube (Implementation)
- **Location**: `ToyEngine/src/ToyEngine/platform/opengl/opengl_texture_cube.h`
- **Purpose**: OpenGL-specific implementation of cube map texture loading and management
- **Features**:
  - Loads 6 individual face images in correct order
  - Proper OpenGL cube map setup (GL_TEXTURE_CUBE_MAP)
  - Appropriate texture parameters to avoid seams
  - Mipmap generation for quality filtering
  - Error handling for failed face loading

## Usage Example

```cpp
#include "ToyEngine/renderer/texture_cube.h"

// Load skybox from 6 face images
Array<std::string, 6> skybox_faces = {
    "assets/textures/skybox/right.jpg",    // +X (right)
    "assets/textures/skybox/left.jpg",     // -X (left)  
    "assets/textures/skybox/top.jpg",      // +Y (top)
    "assets/textures/skybox/bottom.jpg",   // -Y (bottom)
    "assets/textures/skybox/front.jpg",    // +Z (front)
    "assets/textures/skybox/back.jpg"      // -Z (back)
};

auto skybox_texture = TextureCube::Create(skybox_faces, false);
skybox_texture->Bind(0);  // Bind to texture unit 0
```

## Face Ordering

The cube map faces must be provided in this specific order:
1. **+X (right)** - Right face of the cube
2. **-X (left)** - Left face of the cube  
3. **+Y (top)** - Top face of the cube
4. **-Y (bottom)** - Bottom face of the cube
5. **+Z (front)** - Front face of the cube
6. **-Z (back)** - Back face of the cube

This follows the OpenGL standard cube map face ordering.

## Shader Integration

To use the cube map in shaders:

### Vertex Shader
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;  // Ensures skybox is at far plane
}
```

### Fragment Shader
```glsl
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
```

## Technical Details

### OpenGL Configuration
- **Target**: GL_TEXTURE_CUBE_MAP
- **Wrapping**: GL_CLAMP_TO_EDGE (prevents seams between faces)
- **Filtering**: GL_LINEAR_MIPMAP_LINEAR (min), GL_LINEAR (mag)
- **Mipmaps**: Generated automatically for better quality

### Memory Management
- Uses smart pointers (Ref<TextureCube>) for automatic cleanup
- OpenGL texture objects are properly deleted in destructor
- Follows RAII principles

### Error Handling
- Validates that all 6 faces load successfully
- Continues loading remaining faces if one fails
- Logs errors for debugging
- Creates fallback textures to prevent crashes

## Future Enhancements

1. **Single File Loading**: Support for HDR, DDS, and other cube map formats
2. **Runtime Generation**: Procedural cube map generation
3. **Compression**: Support for compressed cube map formats
4. **Streaming**: Large cube map streaming for high-resolution environments

## Integration with Existing Code

The TextureCube implementation:
- ✅ Follows the same patterns as Texture2D
- ✅ Uses the factory pattern with render API abstraction  
- ✅ Integrates with the existing enum system
- ✅ Uses engine's smart pointer and container types
- ✅ Follows engine coding standards and error handling

## Testing

A comprehensive unit test validates:
- Correct factory pattern implementation
- Proper face ordering and storage
- Single file vs. multi-file handling
- Texture binding functionality
- API consistency with Texture2D

Run the test with: `g++ -std=c++17 texture_cube_unit_test.cpp -o test && ./test`