#version 460 core

layout(location = 0) in vec2 iQuadPos;
layout(location = 1) in vec3 iPos;
layout(location = 2) in vec3 iNormal;
layout(location = 3) in float iRadius;
layout(location = 4) in vec4 iColor;

uniform bool uDepthPrepass;
uniform vec3 uCamPos;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uRadiusScale;
uniform float uForwardFactor;

out vec2 vUv;
out vec3 vNormal;
out vec3 vColor;

mat3 calcRotation(vec3 Axis, float Angle)
{
	float c = cos(Angle);
	float sub_c = 1.0 - c;
	float s = sin(Angle);
	return mat3(
		vec3(c + pow(Axis.x, 2.0) * sub_c,
			Axis.y * Axis.x * sub_c + Axis.z * s,
			Axis.z * Axis.x * sub_c - Axis.y * s),
		vec3(Axis.x * Axis.y * sub_c - Axis.z * s,
			c + pow(Axis.y, 2.0) * sub_c,
			Axis.z * Axis.y * sub_c + Axis.x * s),
		vec3(Axis.x * Axis.z * sub_c + Axis.y * s,
			Axis.y * Axis.z * sub_c - Axis.x * s,
			c + pow(Axis.z, 2.0) * sub_c));
}

void main()
{
	vUv = 2.0 * iQuadPos.xy;
	vNormal = normalize(iNormal);
	vColor = iColor.xyz;

	mat3 RotMat = mat3(1.0);
	vec3 QuadNormal = vec3(0, 0, 1);
	float ScaledRadius = iRadius * uRadiusScale;
	if (abs(vNormal) != QuadNormal)
	{
		vec3 RotAxis = normalize(cross(QuadNormal, vNormal));
		float RotAngle = acos(dot(QuadNormal, vNormal));
		RotMat = calcRotation(RotAxis, RotAngle);
	}
	vec3 WorldPos = RotMat * ScaledRadius * vec3(iQuadPos, 0.0) + iPos.xyz;
	if (uDepthPrepass)
	{
		vec3 ViewDir = normalize(WorldPos - uCamPos);
		WorldPos += ViewDir * ScaledRadius * uForwardFactor; // why?
	}
	gl_Position = uProjection * uView * vec4(WorldPos, 1.0);
}