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
#include "utils/AssetsPath.h"
#include "utils/FrameBufferHelper.h"

Elaina::CDirShadowMapPass::CDirShadowMapPass() :m_pShaderProgram(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/shadowMapDir.vert",
	CAssetsPath::getAssetsPath() + "shaders/shadowMapDir.frag"
)) {}

void Elaina::CDirShadowMapPass::initV(int vWidth, int vHeight)
{
	m_pFrameBuffer = CFrameBufferHelper::createDepthOnlyFrameBuffer(1024, 1024);
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
