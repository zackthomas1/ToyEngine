#version 330 core

in vec2 texCoords;

struct Material{
    vec3 diffuse;
}; uniform Material material;

out vec4 FragColor;

void main()
{
    vec3 diffuseColor = vec3(0.0, 1.0, 1.0);
    FragColor = vec4(diffuseColor, 1.0);
}