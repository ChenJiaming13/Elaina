#pragma once

namespace Elaina
{
	struct SPlane
	{
		glm::vec3 _Normal = { 0.f, 1.f, 0.f };
		float _Distance = 0.f;

		SPlane(const glm::vec3& vPoint, const glm::vec3& vNormal)
			: _Normal(glm::normalize(vNormal)),
			_Distance(glm::dot(_Normal, vPoint))
		{}

		[[nodiscard]] float getSignedDistanceToPlane(const glm::vec3& vPoint) const
		{
			return glm::dot(_Normal, vPoint) - _Distance;
		}
	};

	struct SFrustum
	{
		SPlane _TopFace;
		SPlane _BottomFace;
		SPlane _RightFace;
		SPlane _LeftFace;
		SPlane _FarFace;
		SPlane _NearFace;
	};

	struct SBounds
	{
		virtual ~SBounds() = default;

		[[nodiscard]] virtual bool isOnFrustum(const SFrustum& vFrustum, const glm::mat4& vModelMatrix) const = 0;
	};

	struct SAABB final : SBounds
	{
		glm::vec3 _Center{ 0.f, 0.f, 0.f };
		glm::vec3 _Extent{ 0.f, 0.f, 0.f };

		SAABB(const glm::vec3& vMinPoint, const glm::vec3& vMaxPoint)
			: _Center{ (vMaxPoint + vMinPoint) * 0.5f }, _Extent{ vMaxPoint.x - _Center.x, vMaxPoint.y - _Center.y, vMaxPoint.z - _Center.z }
		{}

		SAABB(const glm::vec3& vCenter, float vExtentX, float vExtentY, float vExtentZ)
			: _Center{ vCenter }, _Extent{ vExtentX, vExtentY, vExtentZ } {}

		void getVertices(std::array<glm::vec3, 8>& voVertices) const
		{
			voVertices[0] = { _Center.x - _Extent.x, _Center.y - _Extent.y, _Center.z - _Extent.z };
			voVertices[1] = { _Center.x + _Extent.x, _Center.y - _Extent.y, _Center.z - _Extent.z };
			voVertices[2] = { _Center.x - _Extent.x, _Center.y + _Extent.y, _Center.z - _Extent.z };
			voVertices[3] = { _Center.x + _Extent.x, _Center.y + _Extent.y, _Center.z - _Extent.z };
			voVertices[4] = { _Center.x - _Extent.x, _Center.y - _Extent.y, _Center.z + _Extent.z };
			voVertices[5] = { _Center.x + _Extent.x, _Center.y - _Extent.y, _Center.z + _Extent.z };
			voVertices[6] = { _Center.x - _Extent.x, _Center.y + _Extent.y, _Center.z + _Extent.z };
			voVertices[7] = { _Center.x + _Extent.x, _Center.y + _Extent.y, _Center.z + _Extent.z };
		}

		[[nodiscard]] bool isOnFrustum(const SFrustum& vFrustum, const glm::mat4& vModelMatrix) const override
		{
			const glm::vec3 GlobalCenter{ vModelMatrix * glm::vec4(_Center, 1.f) };
			const glm::vec3 Right = glm::vec3(vModelMatrix[0]) * _Extent.x;
			const glm::vec3 Up = glm::vec3(vModelMatrix[1]) * _Extent.y;
			const glm::vec3 Forward = glm::vec3(vModelMatrix[1]) * _Extent.z;

			const float GlobalExtentX = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, Right)) +
				std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, Up)) +
				std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, Forward));

			const float GlobalExtentY = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, Right)) +
				std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, Up)) +
				std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, Forward));

			const float GlobalExtentZ = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, Right)) +
				std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, Up)) +
				std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, Forward));

			const SAABB GlobalAabb(GlobalCenter, GlobalExtentX, GlobalExtentY, GlobalExtentZ);

			return (GlobalAabb.__isOnOrForwardPlane(vFrustum._LeftFace) &&
				GlobalAabb.__isOnOrForwardPlane(vFrustum._RightFace) &&
				GlobalAabb.__isOnOrForwardPlane(vFrustum._TopFace) &&
				GlobalAabb.__isOnOrForwardPlane(vFrustum._BottomFace) &&
				GlobalAabb.__isOnOrForwardPlane(vFrustum._NearFace) &&
				GlobalAabb.__isOnOrForwardPlane(vFrustum._FarFace));
		}

	private:
		//see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
		[[nodiscard]] bool __isOnOrForwardPlane(const SPlane& vPlane) const
		{
			// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
			const float r = _Extent.x * std::abs(vPlane._Normal.x) + _Extent.y * std::abs(vPlane._Normal.y) +
				_Extent.z * std::abs(vPlane._Normal.z);

			return -r <= vPlane.getSignedDistanceToPlane(_Center);
		}
	};
}
