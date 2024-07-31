#version 330 core
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;

out vec3 vFragPos;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec4 WorldPos = uModel * vec4(iPosition, 1.0f);
    vFragPos = WorldPos.xyz;
    gl_Position = uProjection * uView * WorldPos;
    
    mat3 NormalMatrix = transpose(inverse(mat3(uModel)));
    vNormal = NormalMatrix * iNormal;
}