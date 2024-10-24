#version 460 core

layout(location = 0) in vec3 iPos;

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main()
{
	gl_Position = uViewProjection * uModel * vec4(iPos, 1.0);
}