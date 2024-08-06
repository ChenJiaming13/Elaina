#version 330 core

out vec4 oFragColor;

in vec2 vTexCoords;

// geometry info
uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uDepthTex;

uniform vec3 uLightDir; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;
uniform vec3 uColor;
uniform float uAmbient;
uniform float uSpecular;
uniform float uGlossy;

void main()
{
    if (texture(uDepthTex, vTexCoords).r == 1.0f)
        discard;
    vec3 WorldPos = texture(uPositionTex, vTexCoords).rgb;
    vec3 Normal = texture(uNormalTex, vTexCoords).rgb;
    vec3 Norm = normalize(Normal);

    vec3 Ambient = uAmbient * uLightColor;
  	
    vec3 LightDir = normalize(-uLightDir);
    float Diff = max(dot(Norm, LightDir), 0.0);
    vec3 Diffuse = Diff * uLightColor * uColor;
    
    vec3 ViewDir = normalize(uViewPos - WorldPos);
    vec3 HalfDir = normalize(LightDir + ViewDir);  
    float Spec = pow(max(dot(Norm, HalfDir), 0.0), uGlossy);
    vec3 Specular = uSpecular * Spec * uLightColor;  
        
    vec3 Result = Ambient + Diffuse + Specular;
    oFragColor = vec4(Result, 1.0);
}