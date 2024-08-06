#version 460 core

layout (location = 0) in vec3 iPosition;

out vec3 vTexCoords;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    vTexCoords = iPosition;
    vec4 Pos = uProjection * uView * vec4(iPosition, 1.0);
    gl_Position = Pos.xyww;
}