#version 460 core

in vec4 vFragPos;

uniform vec3 uLightPos;
uniform float uFarPlane;

void main()
{
    float LightDistance = length(vFragPos.xyz - uLightPos);
    
    // map to [0;1] range by dividing by far_plane
    LightDistance = LightDistance / uFarPlane;
    
    // write this as modified depth
    gl_FragDepth = LightDistance;
}