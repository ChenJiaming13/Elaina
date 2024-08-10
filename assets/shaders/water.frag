#version 460 core

out vec4 oFragColor;

in vec3 vWorldPos;
in vec4 vClipPos;
in vec2 vTexCoords;

uniform sampler2D uReflectTex;
uniform sampler2D uRefractTex;
uniform sampler2D uDuDvMap;
uniform float uTilling;
uniform float uWaveLength;
uniform float uMoveFactor;
uniform vec3 uWaterColor;
uniform vec3 uCamPos;

void main()
{
    vec2 NdcPos = (vClipPos.xy / vClipPos.w) / 2.0 + 0.5;
    vec2 ReflectCoords = vec2(NdcPos.x, -NdcPos.y);
    vec2 RefractCoords = vec2(NdcPos.x, NdcPos.y);

    vec2 DuDv1 = (texture(uDuDvMap, vec2(vTexCoords.x + uMoveFactor, vTexCoords.y) * uTilling).xy * 2.0 - 1.0) * uWaveLength;
    vec2 DuDv2 = (texture(uDuDvMap, vec2(-vTexCoords.x + uMoveFactor, vTexCoords.y + uMoveFactor) * uTilling).xy * 2.0 - 1.0) * uWaveLength;
    vec2 DuDv = DuDv1 + DuDv2;

    ReflectCoords += DuDv;
    ReflectCoords.x = clamp(ReflectCoords.x, 0.001, 0.999);
    ReflectCoords.y = clamp(ReflectCoords.y, -0.999, -0.001);
    RefractCoords += DuDv;
    RefractCoords = clamp(RefractCoords, 0.001, 0.999);

    vec3 ReflectColor = texture(uReflectTex, ReflectCoords).xyz;
    vec3 RefractColor = texture(uRefractTex, RefractCoords).xyz;

    vec3 V = normalize(uCamPos - vWorldPos);
    vec3 N = vec3(0.0, 1.0, 0.0);
    float RefractFactor = dot(V, N);
    // RefractFactor = pow(RefractFactor, 10.0);
    vec3 Color = mix(ReflectColor, RefractColor, RefractFactor);
    Color = mix(Color, uWaterColor, 0.2);
    oFragColor = vec4(Color, 1.0);
}