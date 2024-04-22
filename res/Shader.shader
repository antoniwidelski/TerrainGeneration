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

varying vec3 vPos;

void main()
{
	vec4 finalColor;
	float hCol = vPos.y * 3;
	//if (vPos.y < 0.21)
	//{
	//	finalColor = vec4(0.0, 0.3, 1.0, 1.0);
	//}
	//else
	//{
	finalColor = vec4(u_Color.xyz * hCol, 1.0);
	//}
	color = finalColor;
}