#include "pch.h"

using namespace Elaina;

TEST(Test_ShaderProgramManager, NT_CreateGraphicsProgram)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CShaderProgramManager Manager;
	const gl::SGraphicsProgramCreateInfo CreateInfo{
		._VertexShaderPath = "forwardPbr.vert",
		._FragmentShaderPath = "forwardPbr.frag",
	};
	const auto Handle = Manager.createProgram(CreateInfo);
	ASSERT_NE(Handle, gl::INVALID_PROGRAM_HANDLE);

	Manager.useProgram(Handle);
	Manager.setUniform(Handle, "uAlbedo", glm::vec3(1.0f, 0.0f, 1.0f));
	Manager.setUniform(Handle, "uMetallic", 0.1f);
	Manager.setUniform(Handle, "uMetallic", 0.1f);
	Manager.setUniform(Handle, "uInvalid", 1.0f); // invalid uniform

	Manager.cleanup();
}

TEST(Test_ShaderProgramManager, NT_CreateComputeProgram)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CShaderProgramManager Manager;
	const gl::SComputeProgramCreateInfo CreateInfo
	{
		._ComputeShaderPath = "simple.comp"
	};
	const auto Handle = Manager.createProgram(CreateInfo);
	ASSERT_NE(Handle, gl::INVALID_PROGRAM_HANDLE);
	Manager.cleanup();
}

TEST(Test_ShaderProgramManager, DT_InvalidShaderPath)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CShaderProgramManager Manager;
	const gl::SGraphicsProgramCreateInfo CreateInfo{
		._VertexShaderPath = "forwardPbr1.vert",
		._FragmentShaderPath = "forwardPbr.frag",
	};
	const auto Handle = Manager.createProgram(CreateInfo);
	ASSERT_EQ(Handle, gl::INVALID_PROGRAM_HANDLE);
	Manager.cleanup();
}

TEST(Test_ShaderProgramManager, DT_InCompleteProgram)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CShaderProgramManager ProgramManager;
	const gl::SGraphicsProgramCreateInfo CreateInfo{};
	const auto Handle = ProgramManager.createProgram(CreateInfo);
	ASSERT_EQ(Handle, gl::INVALID_PROGRAM_HANDLE);
}
