#include "pch.h"
#include "SkyboxPass.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "base/TextureCube.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "primitive/Primitive.h"
#include "safe.h"
#include "utils/AssetsPath.h"

Elaina::CSkyBoxPass::CSkyBoxPass(const std::array<std::string, 6>& vCubeMapFiles, bool vIsDeferred) :
	m_pCubeMap(nullptr),
	m_pSkyBoxVAO(CPrimitive::createSkyBox()), m_pShaderProgram(CShaderProgram::createShaderProgram(
		CAssetsPath::getAssetsPath() + "shaders\\deferSkyBox.vert",
		CAssetsPath::getAssetsPath() + "shaders\\deferSkyBox.frag")), m_IsDeferred(vIsDeferred)
{
	m_pCubeMap = std::make_shared<CTextureCube>(vCubeMapFiles);
	m_pCubeMap->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_pCubeMap->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_pCubeMap->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_pCubeMap->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_pCubeMap->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Elaina::CSkyBoxPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	if (m_IsDeferred)
	{
		// copy depth buffer
		m_pGeoFrameBuffer->bind(GL_READ_FRAMEBUFFER);
		m_pLitFrameBuffer->bind(GL_DRAW_FRAMEBUFFER);
		GL_SAFE_CALL(glBlitFramebuffer(
			0, 0, m_pGeoFrameBuffer->getWidth(), m_pGeoFrameBuffer->getHeight(),
			0, 0, m_pLitFrameBuffer->getWidth(), m_pLitFrameBuffer->getHeight(),
			GL_DEPTH_BUFFER_BIT, GL_NEAREST
		));
		m_pGeoFrameBuffer->unbind(GL_READ_FRAMEBUFFER);
		m_pLitFrameBuffer->unbind(GL_DRAW_FRAMEBUFFER);
	}
	// render skybox
	m_pLitFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pLitFrameBuffer->getWidth(), m_pLitFrameBuffer->getHeight()));
	const auto& pCamera = vScene->getCamera();
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glDepthFunc(GL_LEQUAL));
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	m_pCubeMap->bind();
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uView", glm::mat4(glm::mat3(pCamera->getViewMatrix())));
	//m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
	m_pShaderProgram->setUniform("uProjection", glm::perspective(90.0f, pCamera->getAspect(), pCamera->getNear(), pCamera->getFar()));
	m_pShaderProgram->setUniform("uSkyBoxTex", 0);
	m_pSkyBoxVAO->bind();
	m_pSkyBoxVAO->draw();
	GL_SAFE_CALL(glDepthFunc(GL_LESS));
}
