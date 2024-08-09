#version 460 core

out vec4 oFragColor;

in vec4 vClipPos;

uniform sampler2D uReflectTex;
uniform sampler2D uRefractTex;

void main()
{
    vec2 NdcPos = (vClipPos.xy / vClipPos.w) / 2.0 + 0.5;
    vec2 ReflectCoords = vec2(NdcPos.x, -NdcPos.y);
    vec2 RefractCoords = vec2(NdcPos.x, NdcPos.y);
    vec3 ReflectColor = texture(uReflectTex, ReflectCoords).xyz;
    vec3 RefractColor = texture(uRefractTex, RefractCoords).xyz;
    vec3 Color = mix(ReflectColor, RefractColor, 0.5);
    // vec3 Color = ReflectColor;
    // vec3 Color = RefractColor;
    oFragColor = vec4(Color, 1.0);
    // oFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}