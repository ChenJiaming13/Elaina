#version 330 core

out vec4 oFragColor;

in vec3 vNormal;

void main()
{
    oFragColor = vec4(vNormal * 0.5 + 0.5, 1.0);
}