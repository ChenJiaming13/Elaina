#version 330 core

out vec4 oFragColor;
  
in vec2 vTexCoords;

uniform sampler2D uMainTex;

void main()
{ 
    oFragColor = vec4(vec3(1.0 - texture(uMainTex, vTexCoords)), 1.0);
}