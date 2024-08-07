#include "pch.h"
#include "PointShadowMapPass.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "core/Mesh.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "light/Light.h"
#include "utils/AssetsPath.h"

Elaina::CPointShadowMapPass::CPointShadowMapPass() :CRenderPass(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders\\shadowMapPoint.vert",
	CAssetsPath::getAssetsPath() + "shaders\\shadowMapPoint.frag",
	CAssetsPath::getAssetsPath() + "shaders\\shadowMapPoint.geom"
)) {}

void Elaina::CPointShadowMapPass::renderV(const std::shared_ptr<CScene>& vScene,
                                          const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, const std::vector<size_t>& vOutputIndices,
                                          size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	const auto& pCurrFrameBuffer = vFrameBuffers[vOutputIndices[vIdxOfPasses]];
	m_Aspect = static_cast<float>(pCurrFrameBuffer->getWidth()) / static_cast<float>(pCurrFrameBuffer->getHeight());
	const auto& pPointLight = vScene->getPointLight();
	__updateShadowTransforms(pPointLight);
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClear(GL_DEPTH_BUFFER_BIT));
	m_pShaderProgram->use();
	for (size_t i = 0; i < m_LightMatrices.size(); ++i)
	{
		m_pShaderProgram->setUniform(std::format("uLightMatrices[{}]", i), m_LightMatrices[i]);
	}
	m_pShaderProgram->setUniform("uFarPlane", pPointLight->_Far);
	m_pShaderProgram->setUniform("uLightPos", pPointLight->_LightPos);

	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<CNode>& vNode){
		m_pShaderProgram->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			pMesh->draw();
		}
	});
}

void Elaina::CPointShadowMapPass::__updateShadowTransforms(const std::shared_ptr<SPointLight>& vPointLight)
{
	m_LightMatrices.clear();
	const glm::mat4 ProjMatrix = glm::perspective(glm::radians(90.0f), m_Aspect, vPointLight->_Near, vPointLight->_Far);
	m_LightMatrices.push_back(ProjMatrix *
		glm::lookAt(vPointLight->_LightPos, vPointLight->_LightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	m_LightMatrices.push_back(ProjMatrix *
		glm::lookAt(vPointLight->_LightPos, vPointLight->_LightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	m_LightMatrices.push_back(ProjMatrix *
		glm::lookAt(vPointLight->_LightPos, vPointLight->_LightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	m_LightMatrices.push_back(ProjMatrix *
		glm::lookAt(vPointLight->_LightPos, vPointLight->_LightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	m_LightMatrices.push_back(ProjMatrix *
		glm::lookAt(vPointLight->_LightPos, vPointLight->_LightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	m_LightMatrices.push_back(ProjMatrix *
		glm::lookAt(vPointLight->_LightPos, vPointLight->_LightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}
