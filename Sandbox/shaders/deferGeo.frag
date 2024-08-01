#version 330 core
layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oNormal;
layout (location = 2) out vec3 oAlbedo;
layout (location = 3) out vec3 oPbrProps;

in vec3 vFragPos;
in vec3 vNormal;

// material parameters
uniform vec3 uAlbedo;
uniform float uMetallic;
uniform float uRoughness;
uniform float uAo;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    oPosition = vFragPos;
    // Also store the per-fragment normals into the gbuffer
    oNormal = normalize(vNormal);
    oAlbedo = uAlbedo;
    oPbrProps = vec3(uMetallic, uRoughness, uAo);
}