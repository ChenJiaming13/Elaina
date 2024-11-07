#version 330 core
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
// layout (location = 2) in vec2 iTexCoords;

// out vec2 vTexCoords;
out vec3 vWorldPos;
out vec3 vNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform bool uEnableClipPlane;
uniform vec4 uClipPlane;

void main()
{
	// vTexCoords = iTexCoords;
	vWorldPos = vec3(uModel * vec4(iPosition, 1.0));
	if (uEnableClipPlane)
	{
		gl_ClipDistance[0] = dot(vec4(vWorldPos, 1.0), uClipPlane);
	}
	vNormal = (transpose(inverse(uModel)) * vec4(iNormal, 0.0)).xyz;   
	gl_Position = uProjection * uView * vec4(vWorldPos, 1.0);
}