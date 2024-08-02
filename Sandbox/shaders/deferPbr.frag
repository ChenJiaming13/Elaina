#version 330 core

out vec4 oFragColor;

in vec2 vTexCoords;

// geometry info
uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uAlbedoTex;
uniform sampler2D uPbrPropsTex;
uniform sampler2D uDepthTex;

// directional light
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform mat4 uLightMatrix;
uniform sampler2D uDirShadowMapTex;

// point lights
uniform vec3 uPointLightColor;
uniform vec3 uPointLightPosition;
uniform float uPointFarPlane;
uniform samplerCube uPointShadowMapTex;

// camera
uniform vec3 uViewPos;

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

vec3 cookTorranceBRDF(vec3 V, vec3 L, vec3 N, vec3 albedo, float roughness, float metallic)
{
    // Cook-Torrance BRDF
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    vec3 H = normalize(V + L);
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
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
    kD *= 1.0 - metallic;

    return kD * albedo / PI + specular;
}

vec3 toneMapping(vec3 color)
{
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    return color;
}

float calcDirShadow(vec3 pWorldPos, vec3 pNormal, vec3 pLightDir)
{
    vec4 LightClipSpacePos = uLightMatrix * vec4(pWorldPos, 1.0);
    vec3 LightNDCPos = LightClipSpacePos.xyz / LightClipSpacePos.w;
    LightNDCPos = LightNDCPos * 0.5 + 0.5;
    float ClosestDepth = texture(uDirShadowMapTex, LightNDCPos.xy).r;
    float CurrDepth = LightNDCPos.z;
    if (CurrDepth > 1.0) return 0.0;
    float Bias = max(0.05 * (1.0 - dot(pNormal, pLightDir)), 0.005);
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
    
    vec3 Lo = vec3(0.0);
    // directional light;
    {
        vec3 L = normalize(-uLightDir);
        vec3 radiance = uLightColor;
        float NdotL = max(dot(N, L), 0.0);
        float DirShadow = calcDirShadow(WorldPos, N, L);
        Lo += cookTorranceBRDF(V, L, N, Albedo, Roughness, Metallic) * radiance * NdotL * (1.0 - DirShadow);
    }
    // point lights
    {
        vec3 L = normalize(uPointLightPosition - WorldPos);
        float distance = length(uPointLightPosition - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = uPointLightColor * attenuation;
        float NdotL = max(dot(N, L), 0.0);
        float PointShadow = calcPointShadow(WorldPos, uPointLightPosition, uPointFarPlane, uPointShadowMapTex);
        Lo += cookTorranceBRDF(V, L, N, Albedo, Roughness, Metallic) * radiance * NdotL * (1.0 - PointShadow);
    }
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * Albedo * Ao;
    vec3 color = ambient + Lo;
    color = toneMapping(color);
    oFragColor = vec4(color, 1.0);
}