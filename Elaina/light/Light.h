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
		// light frustum
		float _Width = 10.0f;
		float _Height = 10.0f;
		float _Near = 0.1f;
		float _Far = 20.0f;

		[[nodiscard]] ELightType getLightType() const override { return ELightType::DIRECTIONAL; }

		[[nodiscard]] glm::mat4 getLightMatrix() const;
	};

	struct SPointLight final : SLight
	{
		float _Near = 0.1f; // shadow frustum
		float _Far = 100.0f; // shadow frustum
		float _Radius;

		[[nodiscard]] ELightType getLightType() const override { return ELightType::POINT; }
	};
}