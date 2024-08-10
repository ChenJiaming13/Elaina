#version 460 core

out vec4 oFragColor;

in vec4 vClipPos;
in vec2 vTexCoords;

uniform sampler2D uReflectTex;
uniform sampler2D uRefractTex;
uniform sampler2D uDuDvMap;
uniform float uTilling;
uniform float uWaveLength;

void main()
{
    vec2 NdcPos = (vClipPos.xy / vClipPos.w) / 2.0 + 0.5;
    vec2 ReflectCoords = vec2(NdcPos.x, -NdcPos.y);
    vec2 RefractCoords = vec2(NdcPos.x, NdcPos.y);

    vec2 DuDv = texture(uDuDvMap, vTexCoords * uTilling).xy;
    DuDv = (DuDv * 2.0 - 1.0) * uWaveLength;
    ReflectCoords += DuDv;
    ReflectCoords.x = clamp(ReflectCoords.x, 0.001, 0.999);
    ReflectCoords.y = clamp(ReflectCoords.y, -0.999, -0.001);
    RefractCoords += DuDv;
    RefractCoords = clamp(RefractCoords, 0.001, 0.999);

    vec3 ReflectColor = texture(uReflectTex, ReflectCoords).xyz;
    vec3 RefractColor = texture(uRefractTex, RefractCoords).xyz;
    vec3 Color = mix(ReflectColor, RefractColor, 0.5);
    oFragColor = vec4(Color, 1.0);
}