#pragma once

#include <glm/glm.hpp>

namespace Elaina
{
	enum class EMaterialType
	{
		UNKNOWN,
		PBR,
		PHONG
	};

	struct SMaterial
	{
		virtual EMaterialType getMaterialType() const { return EMaterialType::UNKNOWN; }
	};

	struct SPbrMaterial : public SMaterial
	{
		EMaterialType getMaterialType() const override { return EMaterialType::PBR; }
		glm::vec3 _Albedo;
		float _Metallic;
		float _Roughness;
		float _Ao;
	};

	struct SPhongMaterial : public SMaterial
	{
		EMaterialType getMaterialType() const override { return EMaterialType::PHONG; }
		glm::vec3 _Color;
		float _Ambient;
		float _Specular;
		float _Glossy;
	};
}