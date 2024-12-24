#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include "gl/GeometryManager.h"

namespace Elaina
{
	class CBasicGeometry
	{
	public:
		~CBasicGeometry();

		explicit CBasicGeometry(gl::CBufferManager* vBufferManager, gl::CGeometryManager* vGeometryManager)
			:m_pBufferManager(vBufferManager), m_pGeometryManager(vGeometryManager) {}

		[[nodiscard]] gl::GeometryHandle getOrCreateCube();

	private:
		enum class EBasicGeometryType : std::uint8_t
		{
			CUBE,
			PLANE,
			QUAD,
			SPHERE
		};

		gl::GeometryHandle __createGeometry(
			float* vVertices,
			unsigned int* vIndices,
			size_t vVerticesCount,
			size_t vIndicesCount,
			const std::vector<int>& vLayouts,
			GLenum vPrimitive
		) const;

		gl::CBufferManager* m_pBufferManager;
		gl::CGeometryManager* m_pGeometryManager;
		std::unordered_map<EBasicGeometryType, gl::GeometryHandle> m_TypeToHandleMap;
	};
}
