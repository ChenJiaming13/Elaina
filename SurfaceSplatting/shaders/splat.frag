#version 460 core

#define M_PI 3.1415926535897932384626433832795

in vec2 vUv;
in vec3 vNormal;
in vec3 vColor;

uniform bool uDepthPrepass;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec3 oNormal;

void main()
{
	float Len = length(vUv);
	if (Len > 1.0)
    {
		discard;
	}
	if (!uDepthPrepass)
    {
		float Opacity = 1.0 / sqrt(2.0 * M_PI) * exp(-pow(Len * 2.5, 2.0)/2.0);
		oColor = vec4(vColor * Opacity, Opacity);
		// oColor = vec4(vColor, 1.0); // test
		oNormal = Opacity * vNormal;
	}
}