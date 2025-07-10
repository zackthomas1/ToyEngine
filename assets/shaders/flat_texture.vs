#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Uniform buffer block for matrices
layout (std140) uniform Matrices
{
    mat4 uView;
    mat4 uProjection;
    mat4 uModel;
};

out vec2 texCoords;

void main()
{
    // texture coordinates
    texCoords = aTexCoords;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}