#version 330 core

uniform sampler2D u_Sampler;
uniform float u_Time;

uniform vec3 u_DirLight;
uniform vec3 u_EyePosition;

in vec2 f_TexCoord;
in vec3 f_Normal;
in vec3 f_WorldPosition;

out vec4 o_Color;

const vec3 AMBIENT_CLR = vec3(0.05f, 0.15f, 0.10f);

void main()
{

	vec3 color = texture(u_Sampler, f_TexCoord).rgb;
	
	//Calculate diffuse lighting
	float diffuse = dot(f_Normal, -u_DirLight);
	diffuse = clamp(diffuse, 0.0f, 1.0f);

	//Calculate specular lighting
	vec3 eyeDirection = normalize(f_WorldPosition - u_EyePosition);
	vec3 middle = -normalize(u_DirLight + eyeDirection);
	float specular = dot(middle, f_Normal);
	specular = clamp(specular, 0.0f, 1.0f);
	specular = pow(specular, 128.0f);

	o_Color = vec4(color * (diffuse + specular) +  color * AMBIENT_CLR, 1.0);
};