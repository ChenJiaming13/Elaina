#include "pch.h"
#include "Light.h"

glm::mat4 Elaina::SDirectionalLight::getLightMatrix() const
{
	const glm::mat4 ViewMat = glm::lookAt(_LightPos, _LightPos + _LightDir, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 ProjMat = glm::ortho(-_Width * 0.5f, _Width * 0.5f, -_Height * 0.5f, _Height * 0.5f, _Near, _Far);
	return ProjMat * ViewMat;
}
