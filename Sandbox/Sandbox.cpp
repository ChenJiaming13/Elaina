#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "base/ShaderProgram.h"
#include "base/VertexArrayBuffer.h"
#include "core/ElainaApp.h"
#include "core/Camera.h"
#include "controller/ArcballController.h"
#include "primitive/Primitive.h"
#include "safe.h"

size_t g_IndexOfVAOs = 0;
size_t g_SizeofVAOs = 0;

class CMyInputHandler : public Elaina::CInputHandler
{
public:
	void onKeyDown(int vKey) override
	{
		if (vKey == 'X')
		{
			g_IndexOfVAOs = (g_IndexOfVAOs + 1) % g_SizeofVAOs;
		}
	}
};

int main()
{
	Elaina::CElainaApp App;
	_ASSERTE(App.init(800, 600));

	Elaina::CShaderProgram Program;
	Program.attachShader(Elaina::CShaderProgram::EShaderType::VERTEX, "pbr.vert");
	Program.attachShader(Elaina::CShaderProgram::EShaderType::FRAGMENT, "pbr.frag");
	Program.linkProgram();

	std::vector<std::shared_ptr<Elaina::CVertexArrayBuffer>> VAOs{
		Elaina::CPrimitive::createTorus(),
		Elaina::CPrimitive::createCube(),
		Elaina::CPrimitive::createSphere()
	};
	g_SizeofVAOs = VAOs.size();

	glm::vec3 LightPos = glm::vec3(0.0f, 0.0f, 3.0f);

	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, (float)App.getWidth() / (float)App.getHeight());
	const auto& pCameraController = std::make_shared<Elaina::CArcballController>();
	pCameraController->control(pCamera);
	App.addInputHandler(pCameraController);
	App.addInputHandler(std::make_shared<CMyInputHandler>());

	float LastTime = 0.0f;
	while (!App.shouldClose())
	{
		App.pollEvents();
		float CurrTime = static_cast<float>(glfwGetTime());
		float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		//spdlog::info("FPS: {}", (int)(1.0f / DeltaTime));
		GL_SAFE_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GL_SAFE_CALL(glViewport(0, 0, App.getWidth(), App.getHeight()));
		GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		//glm::mat4 Model = glm::rotate(glm::mat4(1.0f), CurrTime, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 Model(1.0f);
		Program.use();
		Program.setUniform("uModel", Model);
		Program.setUniform("uView", pCamera->getViewMatrix());
		Program.setUniform("uProjection",pCamera->getProjectionMatrix());
		Program.setUniform("uCamPos", pCamera->getWorldPos());
		Program.setUniform("uLightPosition", LightPos);
		Program.setUniform("uLightColor", glm::vec3(10.0f, 10.0f, 10.0f));
		Program.setUniform("uAlbedo", glm::vec3(1.0f, 1.0f, 0.0f));
		Program.setUniform("uMetallic", 0.0f);
		Program.setUniform("uRoughness", 1.0f);
		Program.setUniform("uAo", 1.0f);
		const auto& pVAO = VAOs[g_IndexOfVAOs];
		pVAO->bind();
		GL_SAFE_CALL(glDrawElements(pVAO->getDrawMode(), pVAO->getVerticesCount(), GL_UNSIGNED_INT, 0));
		App.swapBuffers();
	}
	return 0;
}