#include "pch.h"
#include "DirShadowMapPass.h"
#include "base/ShaderProgram.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "core/Mesh.h"
#include "light/Light.h"
#include "safe.h"
#include "utils/AssetsPath.h"

Elaina::CDirShadowMapPass::CDirShadowMapPass() :CRenderPass(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders/shadowMapDir.vert",
	CAssetsPath::getAssetsPath() + "shaders/shadowMapDir.frag"
)) {}

void Elaina::CDirShadowMapPass::renderV(const std::shared_ptr<CScene>& vScene, const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uLightMatrix", calcLightMatrix(vScene->getDirectionalLight()));
	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		m_pShaderProgram->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			pMesh->draw();
		}
	});
}

glm::mat4 Elaina::CDirShadowMapPass::calcLightMatrix(const std::shared_ptr<SDirectionalLight>& vLight) const
{
	const glm::mat4 ViewMat = glm::lookAt(vLight->_LightPos, vLight->_LightPos + vLight->_LightDir, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 ProjMat = glm::ortho(-m_Width * 0.5f, m_Width * 0.5f, -m_Height * 0.5f, m_Height * 0.5f, m_Near, m_Far);
	return ProjMat * ViewMat;
}
