#include "pch.h"

using namespace Elaina;

TEST(Test_VAOManager, NT_CreateVAO)
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

	const gl::SVAOCreateInfo VAOInfo
	{
		._VertexBufferHandle = VertexBufferHandle,
		._IndexBufferHandle = IndexBufferHandle,
		._Layouts = {3, 3, 2}
	};
	gl::CVAOManager VAOManager;
	const auto VAOHandle = VAOManager.createVAO(VAOInfo);
	ASSERT_NE(VAOHandle, gl::INVALID_VAO_HANDLE);
}