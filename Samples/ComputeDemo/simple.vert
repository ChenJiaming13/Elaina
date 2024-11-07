#version 460 core

layout (location = 0) in vec2 iPos;

out vec2 vTexCoord;

void main()
{
    gl_Position = vec4(iPos.xy, 0.0, 1.0);
    vTexCoord = iPos * 0.5 + 0.5;
}