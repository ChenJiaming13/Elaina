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

	struct SDirectionalLight : SLight
	{
		glm::vec3 _LightDir;
	};

	struct SPointLight : SLight
	{
		float _Near = 0.1f; // shadow frustum
		float _Far = 100.0f; // shadow frustum
		float _Radius;
	};
}