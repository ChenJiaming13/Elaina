#version 330 core

out vec4 oFragColor;

in vec3 vWorldPos;

uniform vec3 uPrimaryColor;
uniform vec3 uSecondaryColor;
uniform float uScale;

void main()
{
    int flag = (int(floor(vWorldPos.x * uScale)) + int(floor(vWorldPos.z * uScale))) % 2;
    oFragColor = flag == 0 ? vec4(uPrimaryColor.xyz, 1.0) : vec4(uSecondaryColor.xyz, 1.0);
}