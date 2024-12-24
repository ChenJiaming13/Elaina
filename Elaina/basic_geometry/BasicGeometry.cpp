#include "pch.h"
#include "BasicGeometry.h"
#include "gl/GeometryManager.h"

using namespace Elaina;

CBasicGeometry::~CBasicGeometry()
{
	for (const auto& Handle : m_TypeToHandleMap | std::views::values)
	{
		m_pGeometryManager->deleteGeometry(Handle);
	}
	m_TypeToHandleMap.clear();
}

gl::GeometryHandle CBasicGeometry::getOrCreateCube()
{
	if (m_TypeToHandleMap.contains(EBasicGeometryType::CUBE)) return m_TypeToHandleMap[EBasicGeometryType::CUBE];

	float Vertices[]
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int Indices[36];
	for (unsigned int i = 0; i < 36; ++i)
	{
		Indices[i] = i;
	}

	const gl::GeometryHandle Handle = __createGeometry(
		Vertices, Indices,
		sizeof(Vertices) / sizeof(float), sizeof(Indices) / sizeof(unsigned int),
		{ 3, 3 }, GL_TRIANGLES
	);

	if (Handle != gl::INVALID_GEOMETRY_HANDLE) m_TypeToHandleMap[EBasicGeometryType::CUBE] = Handle;
	return Handle;
}

gl::GeometryHandle CBasicGeometry::__createGeometry(float* vVertices, unsigned int* vIndices, size_t vVerticesCount,
	size_t vIndicesCount, const std::vector<int>& vLayouts, GLenum vPrimitive) const
{
	const gl::SBufferCreateInfo VertexBufferInfo
	{
		._BufferSize = static_cast<GLsizeiptr>(sizeof(float) * vVerticesCount),
		._BufferUsage = GL_DYNAMIC_STORAGE_BIT,
		._pData = vVertices
	};
	const gl::SBufferCreateInfo IndexBufferInfo
	{
		._BufferSize = static_cast<GLsizeiptr>(sizeof(unsigned int) * vIndicesCount),
		._BufferUsage = GL_DYNAMIC_STORAGE_BIT,
		._pData = vIndices
	};
	const auto VertexBufferHandle = m_pBufferManager->createBuffer(VertexBufferInfo);
	if (VertexBufferHandle == gl::INVALID_BUFFER_HANDLE) return gl::INVALID_GEOMETRY_HANDLE;
	const auto IndexBufferHandle = m_pBufferManager->createBuffer(IndexBufferInfo);
	if (IndexBufferHandle == gl::INVALID_BUFFER_HANDLE) return gl::INVALID_GEOMETRY_HANDLE;
	const gl::SGeometryInfo GeometryInfo
	{
		._VertexBufferHandle = VertexBufferHandle,
		._IndexBufferHandle = IndexBufferHandle,
		._Layouts = vLayouts,
		._VerticesCount = vVerticesCount,
		._IndicesCount = vIndicesCount,
		._Primitive = vPrimitive
	};
	return m_pGeometryManager->createGeometry(GeometryInfo);
}
