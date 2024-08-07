#include "pch.h"
#include "ForwardPbrPass.h"
#include "base/ShaderProgram.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "core/Mesh.h"
#include "core/Material.h"
#include "core/Camera.h"
#include "light/Light.h"
#include "safe.h"
#include "utils/AssetsPath.h"

Elaina::CForwardPbrPass::CForwardPbrPass() :CRenderPass(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/forwardPbr.vert",
	CAssetsPath::getAssetsPath() + "shaders/forwardPbr.frag"
)) {}

void Elaina::CForwardPbrPass::renderV(
	const std::shared_ptr<CScene>& vScene, 
	const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, 
	const std::vector<size_t>& vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	
	const auto& pCamera = vScene->getCamera();
	const auto& pDirLight = vScene->getDirectionalLight();
	const glm::vec4 SolidColor = pCamera->getSolidColor();
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClearColor(SolidColor.x, SolidColor.y, SolidColor.z, SolidColor.w));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
	m_pShaderProgram->setUniform("uCamPos", pCamera->getWorldPos());
	m_pShaderProgram->setUniform("uLightDir", pDirLight->_LightDir);
	m_pShaderProgram->setUniform("uLightColor", pDirLight->_LightColor * pDirLight->_LightIntensity);
	
	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
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
