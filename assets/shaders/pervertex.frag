#version 460 core
out vec4 oFragColor;

in vec3 vColor;

void main()
{
    oFragColor = vec4(vColor.xyz, 1.0);
} 