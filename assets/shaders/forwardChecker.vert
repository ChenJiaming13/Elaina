#version 460 core
layout (location = 0) in vec3 iPos;

out vec3 vWorldPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vWorldPos = vec3(uModel * vec4(iPos, 1.0));
    gl_Position = uProjection * uView * vec4(vWorldPos, 1.0);
}