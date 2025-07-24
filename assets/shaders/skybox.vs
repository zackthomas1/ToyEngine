#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform ViewProjectMats{
    mat4 uView;        // Base alignment: 64 bytes, Aligned offset: 0
    mat4 uProjection;  // Base alignment: 64 bytes, Aligned offset: 64
    vec3 uViewPos;     // Base alignment: 16 bytes, Aligned offset: 128
};

out vec3 texCoords; 

void main()
{
    texCoords = aPos; 
    mat3 viewRotation = mat3(uView);
    mat4 viewNoTranslation = mat4(viewRotation);
    vec4 pos    = uProjection * viewNoTranslation * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}