#include "pch.h"
#include <glm/gtc/matrix_transform.hpp>
#include "core/WindowSystem.h"
#include "basic_geometry/BasicGeometry.h"
#include "gl/ShaderProgramManager.h"

using namespace Elaina;

TEST(Test_BasicGeometry, NT_GetOrCreateCube)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	const auto pBufferManager = new gl::CBufferManager;
	const auto pGeometryManager = new gl::CGeometryManager;
	const auto pBasicGeometry = new CBasicGeometry(pBufferManager, pGeometryManager);
	const auto pProgramManager = new gl::CShaderProgramManager;

	const gl::GeometryHandle CubeHandle = pBasicGeometry->getOrCreateCube();
	ASSERT_NE(CubeHandle, gl::INVALID_GEOMETRY_HANDLE);

	const gl::GeometryHandle CubeHandle2 = pBasicGeometry->getOrCreateCube();
	ASSERT_EQ(CubeHandle2, CubeHandle);

	const gl::SGraphicsProgramCreateInfo ProgramCreateInfo{
		._VertexShaderPath = "cube.vert",
		._FragmentShaderPath = "cube.frag",
	};
	const gl::ProgramHandle ProgramHandle = pProgramManager->createProgram(ProgramCreateInfo);

	float Angle = 0.0f;
	while (!WindowSystem.shouldClose())
	{
		core::CWindowSystem::pollEvents();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pProgramManager->useProgram(ProgramHandle);
		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), WindowSystem.getAspect(), 0.1f, 100.0f);
		const glm::mat4 ModelMatrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)), glm::radians(Angle), glm::vec3(1.0f, 1.0f, 1.0f));
		Angle += 0.5f;
		if (Angle >= 360.0f) Angle = 0.0f;
		pProgramManager->setUniform(ProgramHandle, "uTransform", ProjectionMatrix * ModelMatrix);
		pGeometryManager->draw(CubeHandle);

		WindowSystem.swapBuffers();
	}

	delete pBasicGeometry;
	delete pGeometryManager;
	delete pBufferManager;
	delete pProgramManager;
}