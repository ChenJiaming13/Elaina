#include "pch.h"
#include "DeferredGeoPass.h"
#include "base/ShaderProgram.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "core/Node.h"
#include "core/Mesh.h"
#include "core/Material.h"
#include "safe.h"
#include "utils/AssetsPath.h"

Elaina::CDeferredGeoPass::CDeferredGeoPass() :m_pShaderProgram(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/deferGeo.vert",
	CAssetsPath::getAssetsPath() + "shaders/deferGeo.frag"
)), m_pFrameBuffer(nullptr) {}

void Elaina::CDeferredGeoPass::initV(int vWidth, int vHeight)
{
	m_pFrameBuffer = CFrameBuffer::createFrameBuffer(vWidth, vHeight, std::vector<int>(4, 3));
}

void Elaina::CDeferredGeoPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pFrameBuffer->getWidth(), m_pFrameBuffer->getHeight()));
	GL_SAFE_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	const auto& pCamera = vScene->getCamera();
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
	CNode::traverse(vScene->getRootNode(), [this](const std::shared_ptr<CNode>& vNode) {
		m_pShaderProgram->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			if (pMesh->getMaterial()->getMaterialType() != EMaterialType::PBR)
				continue;
			const auto& pMaterial = std::dynamic_pointer_cast<SPbrMaterial>(pMesh->getMaterial());
			m_pShaderProgram->setUniform("uAlbedo", pMaterial->_Albedo);
			m_pShaderProgram->setUniform("uMetallic", pMaterial->_Metallic);
			m_pShaderProgram->setUniform("uRoughness", pMaterial->_Roughness);
			m_pShaderProgram->setUniform("uAo", pMaterial->_Ao);
			pMesh->draw();
		}
	});
}

void Elaina::CDeferredGeoPass::onWindowSizeChangeV(int vWidth, int vHeight)
{
	m_pFrameBuffer->resize(vWidth, vHeight);
}
