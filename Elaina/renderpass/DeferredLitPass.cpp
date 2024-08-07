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
#include "utils/FrameBufferHelper.h"

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
		m_pFrameBuffer = CFrameBufferHelper::createColorAndDepthFrameBuffer(vWidth, vHeight, std::vector{ 3 });
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
	m_pGeoPositionTex->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE1));
	m_pGeoNormalTex->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE2));
	m_pGeoAlbedoTex->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE3));
	m_pGeoPbrPropsTex->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE4));
	m_pGeoDepthTex->bind();
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
}

void Elaina::CDeferredLitPass::onWindowSizeChangeV(int vWidth, int vHeight)
{
	m_pFrameBuffer->resize(vWidth, vHeight);
}
