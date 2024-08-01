#include "pch.h"
#include "DeferredSkyboxPass.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "base/TextureCube.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "primitive/Primitive.h"
#include "safe.h"

Elaina::CDeferredSkyBoxPass::CDeferredSkyBoxPass(const std::shared_ptr<CShaderProgram>& vShaderProgram, const std::shared_ptr<CTextureCube>& vCubeMap, size_t vIdxOfDeferredGeoFB)
	:CRenderPass(vShaderProgram), m_pCubeMap(vCubeMap), m_IdxOfDeferredGeoFB(vIdxOfDeferredGeoFB), m_pSkyBoxVAO(CPrimitive::createSkyBox())
{
}

Elaina::CDeferredSkyBoxPass::~CDeferredSkyBoxPass()
{
	m_pCubeMap.reset();
	m_pSkyBoxVAO.reset();
}

void Elaina::CDeferredSkyBoxPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	
	// copy depth buffer
	const auto& pDeferredGeoFB = vFrameBuffers[m_IdxOfDeferredGeoFB];
	const auto& pCurrFB = vFrameBuffers[vOutputIndices[vIdxOfPasses]];
	pDeferredGeoFB->bind(GL_READ_FRAMEBUFFER);
	pCurrFB->bind(GL_DRAW_FRAMEBUFFER);
	GL_SAFE_CALL(glBlitFramebuffer(
		0, 0, pDeferredGeoFB->getWidth(), pDeferredGeoFB->getHeight(),
		0, 0, pCurrFB->getWidth(), pCurrFB->getHeight(),
		GL_DEPTH_BUFFER_BIT, GL_NEAREST
	));
	pDeferredGeoFB->unbind(GL_READ_FRAMEBUFFER);
	pCurrFB->unbind(GL_DRAW_FRAMEBUFFER);
	
	// render skybox
	const auto& pCamera = vScene->getCamera();
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glDepthFunc(GL_LEQUAL));
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	m_pCubeMap->bind();
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uView", glm::mat4(glm::mat3(pCamera->getViewMatrix())));
	m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
	m_pShaderProgram->setUniform("uSkyBoxTex", 0);
	m_pSkyBoxVAO->bind();
	m_pSkyBoxVAO->draw();
	GL_SAFE_CALL(glDepthFunc(GL_LESS));
}
