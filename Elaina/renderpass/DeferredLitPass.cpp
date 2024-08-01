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
#include "renderpass/DirShadowMapPass.h"
#include "safe.h"

Elaina::CDeferredLitPass::CDeferredLitPass(
	const std::shared_ptr<CShaderProgram>& vShaderProgram,
	size_t vIdxOfDeferredGeoFB, size_t vIdxOfDirShadowMapFB,
	const std::shared_ptr<CDirShadowMapPass>& vDirShadowMapPass
)
	:CRenderPass(vShaderProgram), m_pQuadVAO(CPrimitive::createQuad()),
	m_IdxOfDeferredGeoFB(vIdxOfDeferredGeoFB), m_IdxOfDirShadowMapFB(vIdxOfDirShadowMapFB),
	m_pDirShadowMapPass(vDirShadowMapPass)
{
}

Elaina::CDeferredLitPass::~CDeferredLitPass()
{
	m_pQuadVAO.reset();
	m_pDirShadowMapPass.reset();
}

void Elaina::CDeferredLitPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	
	const auto& pCamera = vScene->getCamera();
	const auto& pDirLight = vScene->getDirectionalLight();
	glm::vec4 SolidColor = pCamera->getSolidColor();

	GL_SAFE_CALL(glDisable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClearColor(SolidColor.x, SolidColor.y, SolidColor.z, SolidColor.w));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	const auto& pDeferredGeoFB = vFrameBuffers[m_IdxOfDeferredGeoFB];
	const auto& pDirShadowMapFB = vFrameBuffers[m_IdxOfDirShadowMapFB];
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	pDeferredGeoFB->getAttachment(GL_COLOR_ATTACHMENT0)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE1));
	pDeferredGeoFB->getAttachment(GL_COLOR_ATTACHMENT1)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE2));
	pDeferredGeoFB->getAttachment(GL_COLOR_ATTACHMENT2)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE3));
	pDeferredGeoFB->getAttachment(GL_COLOR_ATTACHMENT3)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE4));
	pDeferredGeoFB->getAttachment(GL_DEPTH_ATTACHMENT)->bind();
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE5));
	pDirShadowMapFB->getAttachment(GL_DEPTH_ATTACHMENT)->bind();

	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uPositionTex", 0);
	m_pShaderProgram->setUniform("uNormalTex", 1);
	m_pShaderProgram->setUniform("uAlbedoTex", 2);
	m_pShaderProgram->setUniform("uPbrPropsTex", 3);
	m_pShaderProgram->setUniform("uDepthTex", 4);
	m_pShaderProgram->setUniform("uDirShadowMapTex", 5);
	m_pShaderProgram->setUniform("uViewPos", pCamera->getWorldPos());
	m_pShaderProgram->setUniform("uLightDir", pDirLight->_LightDir);
	m_pShaderProgram->setUniform("uLightColor", pDirLight->_LightColor);
	m_pShaderProgram->setUniform("uLightMatrix", m_pDirShadowMapPass->calcLightMatrix(pDirLight));

	m_pQuadVAO->bind();
	m_pQuadVAO->draw();
}
