#version 460 core

out vec4 oFragColor;

in vec4 vColor;

void main()
{
	oFragColor = vColor;
}