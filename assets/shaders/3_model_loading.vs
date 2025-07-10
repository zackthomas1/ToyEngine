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

out vec3 fragPos; 
out vec3 normal; 
out vec2 texCoords; 

void main()
{
    // For compatibility, use the uniform buffer matrices
    mat4 model = uModel;
    mat4 view = uView;
    mat4 projection = uProjection;

    fragPos = (model * vec4(aPos, 1.0)).xyz; 

    mat3 normalMat = mat3(transpose(inverse(model))); 
    normal = normalMat * aNormal; 
    
    // texture coordinates
    texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}