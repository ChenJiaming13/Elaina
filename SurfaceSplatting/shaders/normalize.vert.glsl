#version 460 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexCoords;

out vec2 vTexCoords;

void main()
{
	vTexCoords = iTexCoords;
	gl_Position = vec4(iPos, 0.0, 1.0);
}