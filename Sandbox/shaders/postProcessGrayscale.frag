#version 330 core

out vec4 oFragColor;
  
in vec2 vTexCoords;

uniform sampler2D uMainTex;

void main()
{ 
    vec4 FragColor = texture(uMainTex, vTexCoords);
    float Average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    oFragColor = vec4(Average, Average, Average, 1.0);
}