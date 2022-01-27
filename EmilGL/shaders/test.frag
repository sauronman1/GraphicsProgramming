#version 330 core

uniform sampler2D u_Sampler;
uniform float u_Time;

in vec2 f_TexCoord;
in vec3 f_Normal;

out vec4 o_Color;

void main()
{
	
	o_Color = texture(u_Sampler, f_TexCoord);
	o_Color = vec4(f_Normal, 1.0);
};