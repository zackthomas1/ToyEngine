#version 330 core

in vec2 texCoords;

struct Material{

    // Note: sampler2D is an opaque type and can only be used as a uniform.
    // Instantiating this struct outside of a uniform may cause GLSL errors.
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};
 uniform Material material;

out vec4 FragColor;

void main()
{
    vec3 diffuseColor   = texture(material.texture_diffuse1, texCoords).rgb;
    vec3 specularColor  = texture(material.texture_specular1, texCoords).rgb;
    FragColor = vec4(diffuseColor, 1.0);
}