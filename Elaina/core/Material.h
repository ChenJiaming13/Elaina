#pragma once

namespace Elaina
{
	enum class EMaterialType
	{
		UNKNOWN,
		PBR,
		PHONG,
		CHECKER,
		WATER,
	};

	struct SMaterial
	{
		virtual ~SMaterial() = default;
		[[nodiscard]] virtual EMaterialType getMaterialType() const { return EMaterialType::UNKNOWN; }
	};

	struct SPbrMaterial final : SMaterial
	{
		[[nodiscard]] EMaterialType getMaterialType() const override { return EMaterialType::PBR; }
		glm::vec3 _Albedo;
		float _Metallic;
		float _Roughness;
		float _Ao;
	};

	struct SPhongMaterial final : SMaterial
	{
		[[nodiscard]] EMaterialType getMaterialType() const override { return EMaterialType::PHONG; }
		glm::vec3 _Color;
		float _Ambient;
		float _Specular;
		float _Glossy;
	};

	struct SCheckerMaterial final : SMaterial
	{
		[[nodiscard]] EMaterialType getMaterialType() const override { return EMaterialType::CHECKER; }
		glm::vec3 _PrimaryColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 _SecondaryColor = glm::vec3(0.5f, 0.5f, 0.5f);
		float _Scale = 1.0f;
	};

	struct SWaterMaterial final : SMaterial
	{
		[[nodiscard]] EMaterialType getMaterialType() const override { return EMaterialType::WATER; }
	};
}