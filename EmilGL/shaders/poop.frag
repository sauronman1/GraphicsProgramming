#version 330 core
in vec3 f_Color;
out vec4 o_Color;

uniform float u_Time;

void main()
{
	float pulse = cos(u_Time * 10.0) * 0.4 + 0.5;
	o_Color = vec4(f_Color * pulse, 1.0f);
}