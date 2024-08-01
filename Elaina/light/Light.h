#pragma once

#include <glm/glm.hpp>

namespace Elaina
{
	struct SLight
	{
		glm::vec3 _LightColor;
		float _LightIntensity;
		glm::vec3 _LightPos;
	};

	struct SDirectionalLight : public SLight
	{
		glm::vec3 _LightDir;
	};
}