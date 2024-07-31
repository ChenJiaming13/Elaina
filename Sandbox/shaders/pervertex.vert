#version 460 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;

out vec3 vColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uLightDir; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;
uniform vec3 uColor;
uniform float uAmbient;
uniform float uSpecular;
uniform float uGlossy;

void main()
{
    vec3 WorldPos = vec3(uModel * vec4(iPos, 1.0));

    // todo normal matrix
    vec3 Ambient = uAmbient * uLightColor;
  	
    vec3 Norm = normalize(iNormal);
    vec3 LightDir = normalize(-uLightDir);
    float Diff = max(dot(Norm, LightDir), 0.0);
    vec3 Diffuse = Diff * uLightColor * uColor;
    
    vec3 ViewDir = normalize(uViewPos - WorldPos);
    vec3 HalfDir = normalize(LightDir + ViewDir);  
    float Spec = pow(max(dot(Norm, HalfDir), 0.0), uGlossy);
    vec3 Specular = uSpecular * Spec * uLightColor;  
        
    vColor = Ambient + Diffuse + Specular;

    gl_Position = uProjection * uView * vec4(WorldPos, 1.0);
}