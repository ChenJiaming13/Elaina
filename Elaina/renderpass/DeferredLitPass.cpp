#include "pch.h"
#include "DeferredLitPass.h"
#include "base/Texture2D.h"
#include "base/ShaderProgram.h"
#include "base/Framebuffer.h"
#include "base/VertexArrayObject.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "light/Light.h"
#include "primitive/Primitive.h"
#include "safe.h"
#include "utils/AssetsPath.h"

Elaina::CDeferredLitPass::CDeferredLitPass(bool vIsFinalPass):
	m_pShaderProgram(CShaderProgram::createShaderProgram(
		CAssetsPath::getAssetsPath() + "shaders\\deferPbr.vert",
		CAssetsPath::getAssetsPath() + "shaders\\deferPbr.frag")),
	m_pQuadVAO(CPrimitive::createQuad()),
	m_pFrameBuffer(nullptr),
	m_IsFinalPass(vIsFinalPass),
	m_EnablePCF(false), m_HalfSizePCF(1)
{}

Elaina::CDeferredLitPass::~CDeferredLitPass()
{
	m_pQuadVAO.reset();
	m_pFrameBuffer.reset();
	m_pShaderProgram.reset();
}

void Elaina::CDeferredLitPass::initV(int vWidth, int vHeight)
{
	if (m_IsFinalPass)
		m_pFrameBuffer = CFrameBuffer::getDefaultFrameBuffer();
	else
		m_pFrameBuffer = CFrameBuffer::createFrameBuffer(vWidth, vHeight, std::vector{ 3 });
}

void Elaina::CDeferredLitPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pFrameBuffer->getWidth(), m_pFrameBuffer->getHeight()));
	const auto& pCamera = vScene->getCamera();
	const auto& pDirLight = vScene->getDirectionalLight();
	const auto& pPointLight = vScene->getPointLight();
	const glm::vec4 SolidColor = pCamera->getSolidColor();

	GL_SAFE_CALL(glDisable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClearColor(SolidColor.x, SolidColor.y, SolidColor.z, SolidColor.w));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	m_pGeoFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE1));
	m_pGeoFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT1)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE2));
	m_pGeoFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT2)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE3));
	m_pGeoFrameBuffer->getAttachment(GL_COLOR_ATTACHMENT3)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE4));
	m_pGeoFrameBuffer->getAttachment(GL_DEPTH_ATTACHMENT)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE5));
	m_pDirShadowMapTex->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE6));
	m_pPointShadowMapTex->bind();

	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uPositionTex", 0);
	m_pShaderProgram->setUniform("uNormalTex", 1);
	m_pShaderProgram->setUniform("uAlbedoTex", 2);
	m_pShaderProgram->setUniform("uPbrPropsTex", 3);
	m_pShaderProgram->setUniform("uDepthTex", 4);
	m_pShaderProgram->setUniform("uDirShadowMapTex", 5);
	m_pShaderProgram->setUniform("uPointShadowMapTex", 6);
	m_pShaderProgram->setUniform("uViewPos", pCamera->getWorldPos());
	m_pShaderProgram->setUniform("uLightDir", pDirLight->_LightDir);
	m_pShaderProgram->setUniform("uLightColor", pDirLight->_LightColor * pDirLight->_LightIntensity);
	m_pShaderProgram->setUniform("uLightMatrix", pDirLight->getLightMatrix());
	m_pShaderProgram->setUniform("uEnablePCF", m_EnablePCF);
	m_pShaderProgram->setUniform("uHalfSizePCF", m_HalfSizePCF);
	m_pShaderProgram->setUniform("uPointLightColor", pPointLight->_LightColor * pPointLight->_LightIntensity);
	m_pShaderProgram->setUniform("uPointLightPosition", pPointLight->_LightPos);
	m_pShaderProgram->setUniform("uPointFarPlane", pPointLight->_Far);

	m_pQuadVAO->bind();
	m_pQuadVAO->draw();

	// copy depth buffer from geo frame buffer
	CFrameBuffer::blit(m_pGeoFrameBuffer, m_pFrameBuffer, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void Elaina::CDeferredLitPass::onWindowSizeChangeV(int vWidth, int vHeight)
{
	m_pFrameBuffer->resize(vWidth, vHeight);
}

bool Elaina::CDeferredLitPass::validateV() const
{
	if (m_pGeoFrameBuffer == nullptr)
	{
		spdlog::error("[DeferredLitPass] Deferred Geo frame buffer miss!");
		return false;
	}
	if (m_pDirShadowMapTex == nullptr)
	{
		spdlog::error("[DeferredLitPass] Dir shadow map miss");
		return false;
	}
	if (m_pPointShadowMapTex == nullptr)
	{
		spdlog::error("[DeferredLitPass] Point shadow map miss");
		return false;
	}
	return true;
}
