#version 460 core
out vec4 oFragColor;

uniform vec3 uLightColor;

void main()
{
    oFragColor = vec4(uLightColor.xyz, 1.0);
}