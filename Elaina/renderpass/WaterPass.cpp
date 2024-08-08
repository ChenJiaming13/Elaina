#include "pch.h"
#include "WaterPass.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "core/Camera.h"
#include "core/Material.h"
#include "core/Mesh.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "utils/AssetsPath.h"

Elaina::CWaterPass::CWaterPass() :m_pShaderProgram(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/forwardWater.vert",
	CAssetsPath::getAssetsPath() + "shaders/forwardWater.frag"
)) {}

void Elaina::CWaterPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pLitFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pLitFrameBuffer->getWidth(), m_pLitFrameBuffer->getHeight()));
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

bool Elaina::CWaterPass::validateV() const
{
	if (m_pLitFrameBuffer == nullptr)
	{
		spdlog::error("[WaterLit Pass] Lit frame buffer miss");
		return false;
	}
	return true;
}
