#include <iostream>
#include "types.h"
#include "helper.h"
#include "safe.h"
#include "../vendor/imgui/imgui.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/Texture.h"
#include "base/VertexArrayObject.h"
#include "controller/ArcballController.h"
#include "core/GlfwWindow.h"
#include "primitive/Primitive.h"
#include "ui/ImGui.h"

static std::shared_ptr<Elaina::CFrameBuffer> g_SplatFrameBuffer = nullptr;
static std::shared_ptr<Elaina::CFrameBuffer> g_DefaultFrameBuffer = nullptr;

class CMyHandler final : public Elaina::CInputHandler
{
public:
	void onWindowSizeChangeV(int vWidth, int vHeight) override
	{
		g_SplatFrameBuffer->resize(vWidth, vHeight);
		g_DefaultFrameBuffer->resize(vWidth, vHeight);
	}
};

int main()
{
	Elaina::CGlfwWindow App;
	_ASSERTE(App.init(800, 600, "Surface Splatting"));
	Elaina::CFrameBuffer::initDefaultFrameBuffer(App.getWidth(), App.getHeight(), 0);
	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, static_cast<float>(App.getWidth()) / static_cast<float>(App.getHeight()));
	const auto& pCameraController = std::make_shared<Elaina::CArcballController>();
	pCameraController->control(pCamera);
	App.addInputHandler(pCameraController);
	App.addInputHandler(std::make_shared<CMyHandler>());

	std::vector<SSurfel> Surfels;
	std::cout << loadRsfFile("painted_santa_kd.rsf", Surfels);

	const GLuint VAO = createVAO(Surfels);
	const auto& pQuadVAO = Elaina::CPrimitive::createQuad();
	const auto& pSplatProgram = Elaina::CShaderProgram::createShaderProgram(
		"shaders/splat.vert",
		"shaders/splat.frag"
	);
	const auto& pNormalizeProgram = Elaina::CShaderProgram::createShaderProgram(
		"shaders/normalize.vert",
		"shaders/normalize.frag"
	);
	g_SplatFrameBuffer = Elaina::CFrameBuffer::createFrameBuffer(App.getWidth(), App.getHeight(), std::vector{ 4, 4 });
	g_DefaultFrameBuffer = Elaina::CFrameBuffer::getDefaultFrameBuffer();

	while (!App.shouldClose())
	{
		App.pollEvents();
		Elaina::CImGui::beginFrame();

		g_SplatFrameBuffer->bind();
		GL_SAFE_CALL(glViewport(0, 0, g_SplatFrameBuffer->getWidth(), g_SplatFrameBuffer->getHeight()));
		GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
		GL_SAFE_CALL(glEnable(GL_BLEND));
		GL_SAFE_CALL(glBlendFunc(GL_ONE, GL_ONE));
		GL_SAFE_CALL(glClearDepth(1.0f));
		GL_SAFE_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		pSplatProgram->use();
		pSplatProgram->setUniform("uCamPos", pCamera->getWorldPos());
		pSplatProgram->setUniform("uView", pCamera->getViewMatrix());
		pSplatProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
		static float RadiusScale = 2.5f;
		pSplatProgram->setUniform("uRadiusScale", RadiusScale);
		static float ForwardFactor = 0.5f;
		pSplatProgram->setUniform("uForwardFactor", ForwardFactor);
		//pSplatProgram->setUniform("uRadiusScale", 1.0f);
		GL_SAFE_CALL(glBindVertexArray(VAO));

		pSplatProgram->setUniform("uDepthPrepass", 1);
		GL_SAFE_CALL(glDepthMask(GL_TRUE));
		GL_SAFE_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
		GL_SAFE_CALL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(Surfels.size())));

		pSplatProgram->setUniform("uDepthPrepass", 0);
		GL_SAFE_CALL(glDepthMask(GL_FALSE));
		GL_SAFE_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
		GL_SAFE_CALL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(Surfels.size())));

		g_DefaultFrameBuffer->bind();
		GL_SAFE_CALL(glViewport(0, 0, g_DefaultFrameBuffer->getWidth(), g_DefaultFrameBuffer->getHeight()));
		GL_SAFE_CALL(glDisable(GL_BLEND));
		GL_SAFE_CALL(glDisable(GL_DEPTH_TEST));
		GL_SAFE_CALL(glDepthMask(GL_TRUE));
		GL_SAFE_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
		static glm::vec4 BackgroundColor = glm::vec4(.1f, 0.2f, 0.3f, 1.0f);
		GL_SAFE_CALL(glClearColor(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, BackgroundColor.a));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
		g_SplatFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
		GL_SAFE_CALL(glActiveTexture(GL_TEXTURE1));
		g_SplatFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT1)->bind();
		pNormalizeProgram->use();
		pNormalizeProgram->setUniform("uColorTex", 0);
		pNormalizeProgram->setUniform("uNormalTex", 1);
		pNormalizeProgram->setUniform("uCamDir", pCamera->getFront());
		static glm::vec3 LightDir1 = glm::vec3(0.5f, 0.5f, 1.0f);
		static glm::vec3 LightDir2 = glm::vec3(-0.5f, 0.25f, -0.5f);
		pNormalizeProgram->setUniform("uLightDir1", LightDir1);
		pNormalizeProgram->setUniform("uLightDir2", LightDir2);
		pQuadVAO->bind();
		pQuadVAO->draw();

		// ui
		ImGui::DragFloat("Radius Scale", &RadiusScale, 0.01f, 0.01f, 10.0f);
		ImGui::DragFloat("Forward Factor", &ForwardFactor, 0.01f, 0.01f, 10.0f);
		ImGui::ColorEdit4("Background Color", &BackgroundColor.r);
		ImGui::DragFloat3("Light Dir1", &LightDir1.x, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat3("Light Dir2", &LightDir2.x, 0.01f, 0.0f, 1.0f);

		Elaina::CImGui::endFrame();
		App.swapBuffers();
	}
	g_SplatFrameBuffer = nullptr;
	g_DefaultFrameBuffer = nullptr;
	Elaina::CImGui::shutdown();
	return 0;
}
