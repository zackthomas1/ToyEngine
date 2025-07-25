#version 330 core

in vec3 texCoords; 

uniform samplerCube skybox_texture; 

out vec4 FragColor;

void main()
{    
    vec4 skyboxColor    = texture(skybox_texture, texCoords);
    FragColor           = skyboxColor;
    // FragColor = vec4(1.0,0.0,0.0,1.0);
}