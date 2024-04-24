#shader vertex
#version 330

layout (location = 0) in vec4 position;

uniform mat4 u_Projection;
uniform mat4 u_View;
varying vec3 vPos;

void main()
{
	vPos = position.xyz;
	gl_Position = u_Projection * u_View * position;
}

#shader fragment
#version 330 core

out vec4 color;

uniform vec4 u_Color;

uniform float u_AmbientLightIntensity;

varying vec3 vPos;

void main()
{
	vec4 finalColor;
	vec4 heightColor = u_Color * vPos.y;
	float radius = sqrt(vPos.x * vPos.x + vPos.z * vPos.z);
	vec4 radiusColor = radius * u_Color * 2;
	finalColor = (heightColor + radiusColor) / 2;
	color = finalColor;
}