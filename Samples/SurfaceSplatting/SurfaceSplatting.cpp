#include "SurfaceSplatting.h"
#include <imgui.h>
#include "helper.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/Texture.h"
#include "base/VertexArrayObject.h"
#include "core/Camera.h"
#include "primitive/Primitive.h"

void CSurfaceSplatting::initV(int vWidth, int vHeight, const std::vector<SSurfel>& vSurfels)
{
	m_SurfelsSize = static_cast<GLsizei>(vSurfels.size());
	m_SurfelsVAO = __createVAO(vSurfels);
	m_pQuadVAO = Elaina::CPrimitive::createQuad();
	m_pSplatProgram = Elaina::CShaderProgram::createShaderProgram(
		"shaders/splat.vert.glsl",
		"shaders/splat.frag.glsl"
	);
	m_pNormalizeProgram = Elaina::CShaderProgram::createShaderProgram(
		"shaders/normalize.vert.glsl",
		"shaders/normalize.frag.glsl"
	);
	m_pSplatFrameBuffer = Elaina::CFrameBuffer::createFrameBuffer(vWidth, vHeight, std::vector{ 4, 4 });
	m_pDefaultFrameBuffer = Elaina::CFrameBuffer::getDefaultFrameBuffer();
}

void CSurfaceSplatting::onWindowSizeChangeV(int vWidth, int vHeight)
{
	m_pSplatFrameBuffer->resize(vWidth, vHeight);
	m_pDefaultFrameBuffer->resize(vWidth, vHeight);
}

void CSurfaceSplatting::renderV(const std::shared_ptr<Elaina::CCamera>& vCamera)
{
	m_pSplatFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pSplatFrameBuffer->getWidth(), m_pSplatFrameBuffer->getHeight()));
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glEnable(GL_BLEND));
	GL_SAFE_CALL(glBlendFunc(GL_ONE, GL_ONE));
	GL_SAFE_CALL(glClearDepth(1.0f));
	GL_SAFE_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_pSplatProgram->use();
	m_pSplatProgram->setUniform("uCamPos", vCamera->getWorldPos());
	m_pSplatProgram->setUniform("uView", vCamera->getViewMatrix());
	m_pSplatProgram->setUniform("uProjection", vCamera->getProjectionMatrix());
	static float RadiusScale = 2.5f;
	m_pSplatProgram->setUniform("uRadiusScale", RadiusScale);
	static float ForwardFactor = 0.5f;
	m_pSplatProgram->setUniform("uForwardFactor", ForwardFactor);
	GL_SAFE_CALL(glBindVertexArray(m_SurfelsVAO));

	m_pSplatProgram->setUniform("uDepthPrepass", 1);
	GL_SAFE_CALL(glDepthMask(GL_TRUE));
	GL_SAFE_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
	GL_SAFE_CALL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_SurfelsSize));

	m_pSplatProgram->setUniform("uDepthPrepass", 0);
	GL_SAFE_CALL(glDepthMask(GL_FALSE));
	GL_SAFE_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
	GL_SAFE_CALL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_SurfelsSize));

	m_pDefaultFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pDefaultFrameBuffer->getWidth(), m_pDefaultFrameBuffer->getHeight()));
	GL_SAFE_CALL(glDisable(GL_BLEND));
	GL_SAFE_CALL(glDisable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glDepthMask(GL_TRUE));
	GL_SAFE_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
	static glm::vec4 BackgroundColor = glm::vec4(0.1f, 0.2f, 0.3f, 1.0f);
	GL_SAFE_CALL(glClearColor(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, BackgroundColor.a));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	m_pSplatFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE1));
	m_pSplatFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT1)->bind();
	m_pNormalizeProgram->use();
	m_pNormalizeProgram->setUniform("uColorTex", 0);
	m_pNormalizeProgram->setUniform("uNormalTex", 1);
	m_pNormalizeProgram->setUniform("uCamDir", vCamera->getFront());
	static glm::vec3 LightDir1 = glm::vec3(0.5f, 0.5f, 1.0f);
	static glm::vec3 LightDir2 = glm::vec3(-0.5f, 0.25f, -0.5f);
	m_pNormalizeProgram->setUniform("uLightDir1", LightDir1);
	m_pNormalizeProgram->setUniform("uLightDir2", LightDir2);
	m_pQuadVAO->bind();
	m_pQuadVAO->draw();

	// ui
	ImGui::DragFloat("Radius Scale", &RadiusScale, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Forward Factor", &ForwardFactor, 0.01f, 0.01f, 10.0f);
	ImGui::ColorEdit4("Background Color", &BackgroundColor.r);
	ImGui::DragFloat3("Light Dir1", &LightDir1.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Light Dir2", &LightDir2.x, 0.01f, 0.0f, 1.0f);
}

void CSurfaceSplatting::cleanup()
{
	glDeleteVertexArrays(1, &m_SurfelsVAO);
	m_SurfelsVAO = 0;
	m_pQuadVAO.reset();
	m_pSplatProgram.reset();
	m_pNormalizeProgram.reset();
	m_pSplatFrameBuffer.reset();
	m_pDefaultFrameBuffer.reset();
}

GLuint CSurfaceSplatting::__createVAO(const std::vector<SSurfel>& vSurfels)
{
	constexpr float QuadVertices[] = {
		-0.5, -0.5,
		-0.5, 0.5,
		0.5, -0.5,
		0.5, 0.5
	};
	GLuint VAO, QuadVBO, SurfelsVBO;
	GL_SAFE_CALL(glGenVertexArrays(1, &VAO));
	GL_SAFE_CALL(glGenBuffers(1, &QuadVBO));
	GL_SAFE_CALL(glGenBuffers(1, &SurfelsVBO));
	GL_SAFE_CALL(glBindVertexArray(VAO));

	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, QuadVBO));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW));
	GL_SAFE_CALL(glEnableVertexAttribArray(0));
	GL_SAFE_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr)));

	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, SurfelsVBO));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(SSurfel) * vSurfels.size(), vSurfels.data(), GL_STATIC_DRAW));
	configSurfelVAOLayout(1, true);
	GL_SAFE_CALL(glBindVertexArray(0));
	return VAO;
}
