#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "base/ShaderProgram.h"
#include "base/VertexArrayBuffer.h"
#include "core/ElainaApp.h"
#include "primitive/Primitive.h"
#include "safe.h"

int main()
{
	const int Width = 800;
	const int Height = 600;
	Elaina::CElainaApp App;
	_ASSERTE(App.init(Width, Height));

	Elaina::CShaderProgram Program;
	Program.attachShader(Elaina::CShaderProgram::EShaderType::VERTEX, "pbr.vert");
	Program.attachShader(Elaina::CShaderProgram::EShaderType::FRAGMENT, "pbr.frag");
	Program.linkProgram();

	const auto& pVAO = Elaina::CPrimitive::createTorus();

	glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 LightPos = glm::vec3(0.0f, 0.0f, 3.0f);

	float LastTime = 0.0f;
	while (!App.shouldClose())
	{
		App.pollEvents();
		float CurrTime = static_cast<float>(glfwGetTime());
		float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		spdlog::info("FPS: {}", (int)(1.0f / DeltaTime));
		
		GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glm::mat4 Model = glm::rotate(glm::mat4(1.0f), CurrTime, glm::vec3(1.0f, 1.0f, 1.0f));
		Program.use();
		Program.setUniform("uModel", Model);
		Program.setUniform("uView", glm::lookAt(
			CamPos, 
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 1.0f, 0.0f)));
		Program.setUniform("uProjection", glm::perspective(glm::radians(45.0f), (float)Width/(float)Height, 0.1f, 100.0f));
		Program.setUniform("uCamPos", CamPos);
		Program.setUniform("uLightPosition", LightPos);
		Program.setUniform("uLightColor", glm::vec3(10.0f, 10.0f, 10.0f));
		Program.setUniform("uAlbedo", glm::vec3(1.0f, 1.0f, 0.0f));
		Program.setUniform("uMetallic", 0.0f);
		Program.setUniform("uRoughness", 1.0f);
		Program.setUniform("uAo", 1.0f);
		pVAO->bind();
		GL_SAFE_CALL(glDrawElements(pVAO->getDrawMode(), pVAO->getVerticesCount(), GL_UNSIGNED_INT, 0));
		App.swapBuffers();
	}
	return 0;
}