#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform ViewProjectMats{
    mat4 uView;       // Base alignment: 64 bytes, Aligned offset: 0
    mat4 uProjection; // Base alignment: 64 bytes, Aligned offset: 64
    vec3 uViewPos;     // Base alignment: 16 bytes, Aligned offset: 128
};
uniform mat4 uModel;

out vec2 texCoords;

void main()
{
    // texture coordinates
    texCoords = aTexCoords;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}