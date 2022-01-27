#version 330 core
uniform sampler2D u_Sampler;

uniform float u_Time;

in vec3 f_Color;
in vec2 f_TexCoord;
out vec4 o_Color;
void main()
{
	
	o_Color = texture(u_Sampler, f_TexCoord);
	
};