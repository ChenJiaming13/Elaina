#version 460 core
layout (location = 0) in vec3 iPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vWorldPos;
out vec4 vClipPos;
out vec2 vTexCoords;

void main()
{
    vWorldPos = vec3(uModel * vec4(iPos, 1.0));
    vClipPos = uProjection * uView * vec4(vWorldPos, 1.0);
    vTexCoords = iPos.xz + 0.5;
    gl_Position = vClipPos;
}