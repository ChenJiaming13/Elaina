#version 460 core

in vec2 vTexCoords;

uniform sampler2D uColorTex;
uniform sampler2D uNormalTex;
uniform vec3 uCamDir;
uniform vec3 uLightDir1;
uniform vec3 uLightDir2;

out vec4 oFragColor;

void main()
{
	vec4 Color = texture(uColorTex, vTexCoords);
	if (Color.a == 0.0) discard;
	vec3 Normal = texture(uNormalTex, vTexCoords).xyz;
	Normal = normalize(Normal);
	vec3 LightDir1 = normalize(uLightDir1);
	vec3 LightDir2 = normalize(uLightDir2);
	float Intensity = 0.25;
	if (dot(LightDir1, Normal) > 0.0)
	{
		Intensity += dot(LightDir1, Normal);
		vec3 H = normalize(normalize(-uCamDir) + LightDir1);
		float NdotH = dot(H, Normal);
		if (NdotH > 0.0)
		{
			Intensity += pow(NdotH, 40.0);
		}
	}
	if (dot(LightDir2, Normal) > 0.0)
	{
		Intensity += dot(LightDir2, Normal) * 0.5;
	}
	Color.rgb *= Intensity;
	oFragColor = vec4(Color.rgb, 1.0);
}