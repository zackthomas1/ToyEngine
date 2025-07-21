/*
 * Example usage of TextureCube for creating skyboxes
 * 
 * This file demonstrates how to use the new TextureCube class
 * to load cube map textures for skybox rendering.
 */

#include "ToyEngine/renderer/texture_cube.h"

namespace ToyEngine
{
    class SkyboxExample
    {
    public:
        static void LoadSkybox()
        {
            // Example 1: Load cube map from 6 individual face images
            // The order is: +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back)
            Array<std::string, 6> skybox_faces = {
                "assets/textures/skybox/right.jpg",    // +X (right)
                "assets/textures/skybox/left.jpg",     // -X (left)
                "assets/textures/skybox/top.jpg",      // +Y (top)
                "assets/textures/skybox/bottom.jpg",   // -Y (bottom)
                "assets/textures/skybox/front.jpg",    // +Z (front)
                "assets/textures/skybox/back.jpg"      // -Z (back)
            };

            // Create the cube map texture
            auto skybox_texture = TextureCube::Create(skybox_faces, false);

            // Example 2: Load from a single cubemap file (future feature)
            // auto skybox_texture = TextureCube::Create("assets/textures/skybox.hdr", false);

            // Bind the texture to a texture unit for use in shaders
            // skybox_texture->Bind(0);  // Binds to GL_TEXTURE0
        }

        static void RenderSkybox(Ref<TextureCube> skybox_texture)
        {
            // In a real implementation, you would:
            // 1. Disable depth writing
            // 2. Bind skybox shader
            // 3. Bind cube map texture
            // 4. Render a cube or use the existing camera view directions
            
            skybox_texture->Bind(0);  // Bind to texture unit 0
            
            // Example shader usage in GLSL:
            // uniform samplerCube skybox;
            // vec3 direction = normalize(position);
            // vec3 color = texture(skybox, direction).rgb;
        }
    };
}

/*
 * Example GLSL Vertex Shader for Skybox:
 * 
 * #version 330 core
 * layout (location = 0) in vec3 aPos;
 * 
 * out vec3 TexCoords;
 * 
 * uniform mat4 projection;
 * uniform mat4 view;
 * 
 * void main()
 * {
 *     TexCoords = aPos;
 *     vec4 pos = projection * view * vec4(aPos, 1.0);
 *     gl_Position = pos.xyww;  // Trick to ensure skybox is always at far plane
 * }
 * 
 * Example GLSL Fragment Shader for Skybox:
 * 
 * #version 330 core
 * out vec4 FragColor;
 * 
 * in vec3 TexCoords;
 * 
 * uniform samplerCube skybox;
 * 
 * void main()
 * {    
 *     FragColor = texture(skybox, TexCoords);
 * }
 */