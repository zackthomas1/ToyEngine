#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform ViewProjectMats{
    mat4 uView;        // Base alignment: 64 bytes, Aligned offset: 0
    mat4 uProjection;  // Base alignment: 64 bytes, Aligned offset: 64
    vec3 uViewPos;     // Base alignment: 16 bytes, Aligned offset: 128
};
uniform mat4 uModel;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec3 viewPos;
} vs_out;

void main()
{
    // Transform the vertex position from local (model) space to world space
    vs_out.fragPos = (uModel * vec4(aPos, 1.0)).xyz; 

    // Compute the normal matrix to correctly transform normals under non-uniform scaling
    // Note: Calculating mat3(transpose(inverse(model))) per vertex can be expensive.
    // It's often better to compute this on the CPU and pass it as a uniform if the model matrix is static.
    mat3 normalMat = mat3(transpose(inverse(uModel))); 
    
    // Transform the input normal vector to world space
    vs_out.normal = normalMat * aNormal;

    vs_out.texCoords = aTexCoords;

    vs_out.viewPos = uViewPos;

    // Compute the final vertex position in clip space for rasterization
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}