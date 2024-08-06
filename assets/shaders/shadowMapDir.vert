#version 460 core
layout (location = 0) in vec3 iPosition;

uniform mat4 uLightMatrix;
uniform mat4 uModel;

void main()
{
    gl_Position = uLightMatrix * uModel * vec4(iPosition, 1.0);
}