#version 460 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in float iRadius;
layout(location = 3) in vec4 iColor;

uniform mat4 uView;
uniform mat4 uProjection;

out vec4 vColor;

void main()
{
	vColor = iColor;
	gl_Position = uProjection * uView * vec4(iPos.xyz, 1.0);
}