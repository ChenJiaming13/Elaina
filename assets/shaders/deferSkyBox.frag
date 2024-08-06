#version 460 core

out vec4 oFragColor;

in vec3 vTexCoords;

uniform samplerCube uSkyBoxTex;

void main()
{    
    oFragColor = texture(uSkyBoxTex, vTexCoords);
}