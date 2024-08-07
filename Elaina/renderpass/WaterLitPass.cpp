#include "pch.h"
#include "WaterLitPass.h"

#include "base/ShaderProgram.h"
#include "core/Camera.h"
#include "core/Material.h"
#include "core/Mesh.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "utils/AssetsPath.h"

Elaina::CWaterLitPass::CWaterLitPass() :CRenderPass(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/forwardWater.vert",
	CAssetsPath::getAssetsPath() + "shaders/forwardWater.frag"
)) {}

void Elaina::CWaterLitPass::renderV(const std::shared_ptr<CScene>& vScene,
	const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices,
	size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	const auto& pCamera = vScene->getCamera();
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());

	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		m_pShaderProgram->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			if (pMesh->getMaterial()->getMaterialType() != EMaterialType::WATER)
				continue;
			//const auto& pMaterial = std::dynamic_pointer_cast<SPbrMaterial>(pMesh->getMaterial());
			pMesh->draw();
		}
	});
}
