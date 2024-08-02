#include "pch.h"
#include "DeferVisLightPass.h"
#include "primitive/Primitive.h"
#include "safe.h"
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "core/Camera.h"
#include "core/Scene.h"
#include "light/Light.h"

Elaina::CDeferVisLightPass::CDeferVisLightPass(const std::shared_ptr<CShaderProgram>& vShaderProgram)
	:CRenderPass(vShaderProgram), m_pLightVAO(CPrimitive::createSphere())
{
}

Elaina::CDeferVisLightPass::~CDeferVisLightPass()
{
	m_pLightVAO.reset();
}

void Elaina::CDeferVisLightPass::renderV(
	const std::shared_ptr<CScene>& vScene,
	const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, 
	const std::vector<size_t> vOutputIndices, size_t vIdxOfPasses)
{
	CRenderPass::renderV(vScene, vFrameBuffers, vOutputIndices, vIdxOfPasses);
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	const std::vector<std::shared_ptr<SLight>> pLights
	{
		vScene->getDirectionalLight(),
		vScene->getPointLight()
	};
	const auto& pCamera = vScene->getCamera();
	m_pShaderProgram->use();
	m_pShaderProgram->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgram->setUniform("uProjection", pCamera->getProjectionMatrix());
	for (const auto& pLight : pLights)
	{
		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::translate(Model, pLight->_LightPos);
		Model = glm::scale(Model, glm::vec3(0.1f));
		m_pShaderProgram->setUniform("uModel", Model);
		m_pShaderProgram->setUniform("uLightColor", pLight->_LightColor);
		m_pLightVAO->bind();
		m_pLightVAO->draw();
	}
}
