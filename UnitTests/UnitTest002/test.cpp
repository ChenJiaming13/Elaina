#include "pch.h"

using namespace Elaina;

TEST(Test_GeometryManager, NT_CreateGeometry)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	float Vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int Indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	constexpr gl::SBufferCreateInfo VertexBufferInfo
	{
		._BufferSize = sizeof(Vertices),
		._BufferUsage = GL_DYNAMIC_STORAGE_BIT,
		._pData = Vertices
	};
	constexpr gl::SBufferCreateInfo IndexBufferInfo
	{
		._BufferSize = sizeof(Vertices),
		._BufferUsage = GL_DYNAMIC_STORAGE_BIT,
		._pData = Indices
	};

	gl::CBufferManager BufferManager;
	const auto VertexBufferHandle = BufferManager.createBuffer(VertexBufferInfo);
	ASSERT_NE(VertexBufferHandle, gl::INVALID_BUFFER_HANDLE);
	const auto IndexBufferHandle = BufferManager.createBuffer(IndexBufferInfo);
	ASSERT_NE(IndexBufferHandle, gl::INVALID_BUFFER_HANDLE);

	const gl::SGeometryInfo GeometryInfo
	{
		._VertexBufferHandle = VertexBufferHandle,
		._IndexBufferHandle = IndexBufferHandle,
		._Layouts = {3, 3, 2},
		._VerticesCount = 4,
		._IndicesCount = 6,
		._Primitive = GL_TRIANGLES
	};
	gl::CGeometryManager GeometryManager;
	const auto VAOHandle = GeometryManager.createGeometry(GeometryInfo);
	ASSERT_NE(VAOHandle, gl::INVALID_GEOMETRY_HANDLE);
}