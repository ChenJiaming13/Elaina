#version 460 core

in vec2 vTexCoords;

uniform sampler2D uColorTex;

out vec4 oFragColor;

void main()
{
    oFragColor = texture(uColorTex, vTexCoords);
}