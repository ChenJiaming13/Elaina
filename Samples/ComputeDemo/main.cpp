#include <glad/glad.h>
#include "core/WindowSystem.h"
#include "gl/ShaderProgramManager.h"
#include "gl/TextureManager.h"
#include "gl/BufferManager.h"
#include "gl/GeometryManager.h"
#include "gl/FrameBufferManager.h"

using namespace Elaina;

int main()
{
	core::CWindowSystem WindowSystem;
	WindowSystem.init();
	gl::CFrameBufferManager FrameBufferManager;
	gl::CTextureManager TextureManager;
	gl::CShaderProgramManager ProgramManager;
	gl::CBufferManager BufferManager;
	gl::CGeometryManager GeometryManager;

	float Vertices[] = {
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
	};
	unsigned int Indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	gl::SBufferCreateInfo VertexBufferCreateInfo{
		._BufferSize = sizeof(Vertices),
		._BufferUsage = GL_DYNAMIC_STORAGE_BIT,
		._pData = Vertices
	};
	gl::SBufferCreateInfo IndexBufferCreateInfo{
		._BufferSize = sizeof(Indices),
		._BufferUsage = GL_DYNAMIC_STORAGE_BIT,
		._pData = Indices
	};
	auto VertexBufferHandle = BufferManager.createBuffer(VertexBufferCreateInfo);
	auto IndexBufferHandle = BufferManager.createBuffer(IndexBufferCreateInfo);
	gl::SGeometryInfo GeometryInfo
	{
		._VertexBufferHandle = VertexBufferHandle,
		._IndexBufferHandle = IndexBufferHandle,
		._Layouts = { 2 },
		._VerticesCount = 4,
		._IndicesCount = 6,
		._Primitive = GL_TRIANGLES
	};
	gl::GeometryHandle GeometryHandle = GeometryManager.createGeometry(GeometryInfo);
	constexpr int Width = 512;
	constexpr int Height = 512;
	constexpr gl::STexture2DCreateInfo TextureCreateInfo
	{
		._Extent = { Width, Height },
		._MipLevels = 1,
		._InternalFormat = GL_RGBA32F,
		._WrapS = GL_CLAMP_TO_EDGE,
		._WrapT = GL_CLAMP_TO_EDGE,
		._FilterMin = GL_LINEAR,
		._FilterMag = GL_LINEAR,
	};
	const gl::TextureHandle OutputTextureHandle = TextureManager.createTexture(TextureCreateInfo);
	const gl::TextureHandle InputTextureHandle = TextureManager.getOrCreateTexture("texture.png", true);
	TextureManager.bindImage2D(OutputTextureHandle, 0, GL_WRITE_ONLY, 0);
	TextureManager.bindImage2D(InputTextureHandle, 1, GL_READ_ONLY, 0);

	const gl::SComputeProgramCreateInfo ComputeProgramCreateInfo
	{
		._ComputeShaderPath = "simple.comp"
	};
	const gl::ProgramHandle ComputeProgramHandle = ProgramManager.createProgram(ComputeProgramCreateInfo);

	const gl::SGraphicsProgramCreateInfo GraphicsProgramCreateInfo
	{
		._VertexShaderPath = "simple.vert",
		._FragmentShaderPath = "simple.frag"
	};
	const gl::ProgramHandle GraphicsProgramHandle = ProgramManager.createProgram(GraphicsProgramCreateInfo);

	//int WorkGrpCnt[3];
	//glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &WorkGrpCnt[0]);
	//glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &WorkGrpCnt[1]);
	//glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &WorkGrpCnt[2]);

	//int WorkGrpSize[3];
	//glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &WorkGrpSize[0]);
	//glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &WorkGrpSize[1]);
	//glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &WorkGrpSize[2]);

	//int WorkGrpInv;
	//glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &WorkGrpInv);

	while (!WindowSystem.shouldClose())
	{
		core::CWindowSystem::pollEvents();

		ProgramManager.useProgram(ComputeProgramHandle);
		glDispatchCompute(Width, Height, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		FrameBufferManager.bindFrameBuffer(gl::DEFAULT_FRAME_BUFFER_HANDLE);
		TextureManager.bindSampler2D(OutputTextureHandle, 0);
		ProgramManager.useProgram(GraphicsProgramHandle);
		ProgramManager.setUniform(GraphicsProgramHandle, "uTexture", 0);
		GeometryManager.draw(GeometryHandle);

		WindowSystem.swapBuffers();
	}
}