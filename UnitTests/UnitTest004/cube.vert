#version 330 core

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;

uniform mat4 uTransform;

out vec3 vNormal;

void main()
{
    gl_Position = uTransform * vec4(iPosition, 1.0);
    vNormal = iNormal;
}