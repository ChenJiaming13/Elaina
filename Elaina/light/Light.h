#pragma once

#include <glm/glm.hpp>

namespace Elaina
{
	enum class ELightType
	{
		UNKNOWN,
		POINT,
		DIRECTIONAL
	};

	struct SLight
	{
		glm::vec3 _LightColor;
		float _LightIntensity;
		glm::vec3 _LightPos;

		[[nodiscard]] virtual ELightType getLightType() const { return ELightType::UNKNOWN; }
		virtual ~SLight() = default;
	};

	struct SDirectionalLight final : SLight
	{
		glm::vec3 _LightDir;

		[[nodiscard]] ELightType getLightType() const override { return ELightType::DIRECTIONAL; }
	};

	struct SPointLight final : SLight
	{
		float _Near = 0.1f; // shadow frustum
		float _Far = 100.0f; // shadow frustum
		float _Radius;

		[[nodiscard]] ELightType getLightType() const override { return ELightType::POINT; }
	};
}