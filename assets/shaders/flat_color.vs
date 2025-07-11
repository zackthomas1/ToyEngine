#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices{
    mat4 uView;       // Base alignment: 64 bytes, Aligned offset: 0
    mat4 uProjection; // Base alignment: 64 bytes, Aligned offset: 64
};
uniform mat4 uModel;

void main()
{
    // texture coordinates
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}