#version 330 core

in vec2 texCoords;

struct Material{

    // Note: sampler2D is a so called opaque type which means 
    // it be can't instantiated, but only defined as uniforms. 
    // If the struct would be instantiated other than as a uniform 
    // (like a function parameter) GLSL could throw strange errors; 
    // the same thus applies to any struct holding such opaque types.
    sampler2D texture_diffuse1;

}; uniform Material material;

out vec4 FragColor;

void main()
{
    vec3 diffuseColor = texture(material.texture_diffuse1, texCoords).rgb;
    FragColor = vec4(diffuseColor, 1.0);
}