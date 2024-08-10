#include "pch.h"
#include "WaterLitPass.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/Texture2D.h"
#include "core/Camera.h"
#include "core/Mesh.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "primitive/Primitive.h"
#include "utils/AssetsPath.h"

Elaina::CWaterLitPass::CWaterLitPass() :
	m_pShaderProgram(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/water.vert",
	CAssetsPath::getAssetsPath() + "shaders/water.frag")
	),
	m_pWaterPlaneVAO(CPrimitive::createPlane()),
	m_pDuDvMap(std::make_shared<CTexture2D>(CAssetsPath::getAssetsPath() + "textures/waterDUDV.png")),
	m_WaterHeight(-3.0f)
{
	m_pDuDvMap->bind();
	m_pDuDvMap->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_pDuDvMap->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	m_pDuDvMap->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_pDuDvMap->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(0.0f, m_WaterHeight, 0.0f));
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(8.0f, 1.0f, 8.0f));
}

void Elaina::CWaterLitPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pLitFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pLitFrameBuffer->getWidth(), m_pLitFrameBuffer->getHeight()));

	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	m_pReflectFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE1));
	m_pRefractFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE2));
	m_pDuDvMap->bind();

	const auto& pCamera = vScene->getCamera();
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uModel", m_ModelMatrix);
	m_pShaderProgram->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
	m_pShaderProgram->setUniform("uReflectTex", 0);
	m_pShaderProgram->setUniform("uRefractTex", 1);
	m_pShaderProgram->setUniform("uDuDvMap", 2);
	m_pShaderProgram->setUniform("uTilling", 6.0f);
	m_pShaderProgram->setUniform("uWaveLength", 0.02f);
	m_pWaterPlaneVAO->bind();
	m_pWaterPlaneVAO->draw();
}

bool Elaina::CWaterLitPass::validateV() const
{
	if (m_pLitFrameBuffer == nullptr)
	{
		spdlog::error("[WaterLit Pass] Lit frame buffer miss");
		return false;
	}
	if (m_pReflectFrameBuffer == nullptr)
	{
		spdlog::error("[WaterLit Pass] Reflect frame buffer miss");
		return false;
	}
	if (m_pRefractFrameBuffer == nullptr)
	{
		spdlog::error("[WaterLit Pass] Refract frame buffer miss");
		return false;
	}
	return true;
}
