#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

uniform vec3 pickedColor;

out vec3 outColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	outColor = pickedColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}