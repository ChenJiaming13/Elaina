#version 460 core
layout (location = 0) in vec3 iPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec4 vClipPos;
out vec2 vTexCoords;

void main()
{
    vClipPos = uProjection * uView * uModel * vec4(iPos, 1.0);
    vTexCoords = iPos.xz + 0.5;
    gl_Position = vClipPos;
}