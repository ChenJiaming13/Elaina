#version 330 core
layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oNormal;

in vec3 vFragPos;
in vec3 vNormal;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    oPosition = vFragPos;
    // Also store the per-fragment normals into the gbuffer
    oNormal = normalize(vNormal);
}