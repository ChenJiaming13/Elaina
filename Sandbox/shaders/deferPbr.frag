#version 330 core

out vec4 oFragColor;

in vec2 vTexCoords;

// geometry info
uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uAlbedoTex;
uniform sampler2D uPbrPropsTex;
uniform sampler2D uDepthTex;
uniform sampler2D uDirShadowMapTex;
uniform samplerCube uPointShadowMapTex;

// material parameters
// uniform vec3 uAlbedo;
// uniform float uMetallic;
// uniform float uRoughness;
// uniform float uAo;

// lights
// uniform vec3 uLightPosition;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform float uLightIntensity;

// point lights
uniform vec3 uPointLightPos;
uniform float uPointFarPlane;

uniform vec3 uViewPos;

uniform mat4 uLightMatrix;

const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float calcDirShadow(vec3 vWorldPos, vec3 vNormal, vec3 vLightDir)
{
    vec4 LightClipSpacePos = uLightMatrix * vec4(vWorldPos, 1.0);
    vec3 LightNDCPos = LightClipSpacePos.xyz / LightClipSpacePos.w;
    LightNDCPos = LightNDCPos * 0.5 + 0.5;
    float ClosestDepth = texture(uDirShadowMapTex, LightNDCPos.xy).r;
    float CurrDepth = LightNDCPos.z;
    if (CurrDepth > 1.0) return 0.0;
    float Bias = max(0.05 * (1.0 - dot(vNormal, vLightDir)), 0.005);
    float Shadow = CurrDepth - Bias > ClosestDepth ? 1.0 : 0.0;
    return Shadow;
}

float calcPointShadow(vec3 pWorldPos, vec3 pPointLightPos, float pFarPlane, samplerCube pShadowMapTex)
{
    vec3 FragToLight = pWorldPos - pPointLightPos;
    float ClosestDepth = texture(pShadowMapTex, FragToLight).r;
    ClosestDepth *= pFarPlane;
    float CurrDepth = length(FragToLight);
    float Bias = 0.05;
    float Shadow = CurrDepth - Bias > ClosestDepth ? 1.0 : 0.0;
    return Shadow;
}

void main()
{
    if (texture(uDepthTex, vTexCoords).r == 1.0f)
        discard;
    vec3 WorldPos = texture(uPositionTex, vTexCoords).rgb;
    vec3 Normal = texture(uNormalTex, vTexCoords).rgb;
    vec3 Albedo = texture(uAlbedoTex, vTexCoords).rgb;
    vec3 PbrProps = texture(uPbrPropsTex, vTexCoords).rgb;
    float Metallic = PbrProps.r;
    float Roughness = PbrProps.g;
    float Ao = PbrProps.b;
    vec3 N = normalize(Normal);
    vec3 V = normalize(uViewPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metallic);

    // reflectance equation
    // calculate per-light radiance
    // vec3 L = normalize(uLightPosition - WorldPos);
    vec3 L = normalize(-uLightDir);
    vec3 H = normalize(V + L);
    // float distance = length(uLightPosition - WorldPos);
    float distance = 1.0;
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = uLightColor * uLightIntensity * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, Roughness);   
    float G   = GeometrySmith(N, V, L, Roughness);      
    vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
       
    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - Metallic;	  

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);        

    // add to outgoing radiance Lo
    vec3 Lo = (kD * Albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
       
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * Albedo * Ao;

    float DirShadow = calcDirShadow(WorldPos, N, L);
    float PointShadow = calcPointShadow(WorldPos, uPointLightPos, uPointFarPlane, uPointShadowMapTex);
    DirShadow = 0.0;
    float Shadow = clamp(DirShadow + PointShadow, 0.0, 1.0);
    vec3 color = ambient + Lo * (1.0 - Shadow);

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    oFragColor = vec4(color, 1.0);
    // oFragColor = texture(uPointShadowMapTex, WorldPos - uPointLightPos);
}