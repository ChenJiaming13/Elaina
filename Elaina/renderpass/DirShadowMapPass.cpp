#include "pch.h"
#include "DirShadowMapPass.h"
#include "base/ShaderProgram.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "core/Mesh.h"
#include "core/Material.h"
#include "light/Light.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/Texture2D.h"
#include "utils/AssetsPath.h"

Elaina::CDirShadowMapPass::CDirShadowMapPass() :m_pShaderProgram(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/shadowMapDir.vert",
	CAssetsPath::getAssetsPath() + "shaders/shadowMapDir.frag"
)) {}

void Elaina::CDirShadowMapPass::initV(int vWidth, int vHeight)
{
	const auto& pDepthTex = std::make_shared<CTexture2D>(1024, 1024, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	pDepthTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	constexpr GLfloat BorderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	pDepthTex->setParameter(GL_TEXTURE_BORDER_COLOR, BorderColor);

	m_pFrameBuffer = std::make_shared<CFrameBuffer>();
	m_pFrameBuffer->create();
	m_pFrameBuffer->bind();
	m_pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex, 0);
	CFrameBuffer::setColorBufferEmpty();
	CFrameBuffer::checkComplete();
	CFrameBuffer::unbind();
}

void Elaina::CDirShadowMapPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pFrameBuffer->getWidth(), m_pFrameBuffer->getHeight()));
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uLightMatrix", vScene->getDirectionalLight()->getLightMatrix());
	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		m_pShaderProgram->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			if (pMesh->getMaterial()->getMaterialType() == EMaterialType::UNKNOWN ||
				pMesh->getMaterial()->getMaterialType() == EMaterialType::WATER)
				continue;
			pMesh->draw();
		}
	});
}

void Elaina::CDirShadowMapPass::getShadowMapSize(int& voWidth, int& voHeight) const
{
	voWidth = m_pFrameBuffer->getWidth();
	voHeight = m_pFrameBuffer->getHeight();
}

void Elaina::CDirShadowMapPass::setShadowMapSize(int vWidth, int vHeight) const
{
	m_pFrameBuffer->resize(vWidth, vHeight);
}
