#version 330 core

layout (location = 0) in vec3 a_pos;		// 
layout (location = 1) in vec2 a_tex_coord;// 

out vec4 vertexPosition;
out vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view; 
uniform mat4 u_projection;

void main()
{
	// pass uniforms to fragment shader
	vertexPosition = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);; 
	texCoord = a_tex_coord;

	gl_Position = u_projection * u_view * u_model * vec4(a_pos.x , a_pos.y, a_pos.z, 1.0);
}