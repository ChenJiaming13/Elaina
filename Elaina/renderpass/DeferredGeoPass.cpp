#include "pch.h"
#include "DeferredGeoPass.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "core/Node.h"
#include "core/Mesh.h"
#include "core/Material.h"
#include "safe.h"
#include "utils/AssetsPath.h"

Elaina::CDeferredGeoPass::CDeferredGeoPass() :CRenderPass(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/deferGeo.vert",
	CAssetsPath::getAssetsPath() + "shaders/deferGeo.frag"
)) {}

void Elaina::CDeferredGeoPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	const auto& pCamera = vScene->getCamera();
	GL_SAFE_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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
