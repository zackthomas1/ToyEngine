#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec4 vertexPosition;
in vec2 texCoord;

uniform vec4 	u_periodic_brightness;
uniform float 	u_alpha_tex;
uniform vec2 	u_pos_tex; 
uniform float 	u_scale_tex;

// texture samplers
uniform sampler2D text_sample_0;	// texture unit = 0
uniform sampler2D text_sample_1; 	// texture unit = 1

void main()
{

	vec2 trans_tex_coords = (texCoord * u_scale_tex) + u_pos_tex;

	vec4 tex0 = texture(text_sample_0, texCoord); 
	vec4 tex1 = texture(text_sample_1, trans_tex_coords);

	vec3 FragRGB = (tex0.xyz * tex0.w) * (1.0 - tex1.w * u_alpha_tex) + (tex1.xyz * tex1.w * u_alpha_tex);
	// vec3 FragRGB = tex1.xyz;
	FragColor = vec4(FragRGB, 1.0);

} 