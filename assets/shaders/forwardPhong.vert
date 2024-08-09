#version 460 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;

out vec3 vWorldPos;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform bool uEnableClipPlane;
uniform vec4 uClipPlane;

void main()
{
    vWorldPos = vec3(uModel * vec4(iPos, 1.0));
    if (uEnableClipPlane)
	{
		gl_ClipDistance[0] = dot(vec4(vWorldPos, 1.0), uClipPlane);
	}
    vNormal = iNormal;
    // todo normal matrix
    gl_Position = uProjection * uView * vec4(vWorldPos, 1.0);
}