#version 460 core

out vec4 oFragColor;

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
    oFragColor = texture(uTexture, vTexCoord);
}