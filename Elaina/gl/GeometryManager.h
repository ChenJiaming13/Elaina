#pragma once

#include <vector>
#include <map>
#include "BufferManager.h"

namespace Elaina::gl
{
	using GeometryHandle = GLuint;

	constexpr auto INVALID_GEOMETRY_HANDLE = std::numeric_limits<GeometryHandle>::max();

	struct SGeometryInfo
	{
		BufferHandle _VertexBufferHandle = INVALID_BUFFER_HANDLE;
		BufferHandle _IndexBufferHandle = INVALID_BUFFER_HANDLE;
		std::vector<int> _Layouts{};
		size_t _VerticesCount = 0;	// num of vertices
		size_t _IndicesCount = 0;	// sizeof(indices) / sizeof(indices[0])
		GLenum _Primitive = GL_TRIANGLES;
	};

	class CGeometryManager
	{
	public:
		GeometryHandle createGeometry(const SGeometryInfo& vInfo); // ABCABC

		void deleteGeometry(GeometryHandle vHandle);

		void draw(GeometryHandle vHandle) const;

	private:
		std::map<GeometryHandle, SGeometryInfo> m_Geometries;
	};
}
