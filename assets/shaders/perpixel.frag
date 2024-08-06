#version 460 core
out vec4 oFragColor;

in vec3 vWorldPos;
in vec3 vNormal;

uniform vec3 uLightDir; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;
uniform vec3 uColor;
uniform float uAmbient;
uniform float uSpecular;
uniform float uGlossy;

void main()
{
    vec3 Ambient = uAmbient * uLightColor;
  	
    vec3 Norm = normalize(vNormal);
    vec3 LightDir = normalize(-uLightDir);
    float Diff = max(dot(Norm, LightDir), 0.0);
    vec3 Diffuse = Diff * uLightColor * uColor;
    
    vec3 ViewDir = normalize(uViewPos - vWorldPos);
    vec3 HalfDir = normalize(LightDir + ViewDir);  
    float Spec = pow(max(dot(Norm, HalfDir), 0.0), uGlossy);
    vec3 Specular = uSpecular * Spec * uLightColor;  
        
    vec3 Result = Ambient + Diffuse + Specular;
    oFragColor = vec4(Result, 1.0);
} 