#include "pch.h"
#include "VisLightPass.h"
#include "primitive/Primitive.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "core/Camera.h"
#include "core/Mesh.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "light/Light.h"
#include "utils/AssetsPath.h"

Elaina::CVisLightPass::CVisLightPass():
	m_pLitFrameBuffer(nullptr),
	m_pShaderProgram(CShaderProgram::createShaderProgram(
		CAssetsPath::getAssetsPath() + "shaders\\visLight.vert",
		CAssetsPath::getAssetsPath() + "shaders\\visLight.frag")),
	m_pPointLightVAO(CPrimitive::createSphere()), m_pArrowNode(std::make_shared<CNode>())
{
	const auto& pCylinderMesh = std::make_shared<CMesh>(CPrimitive::createCylinder(0.2f, 2.0f, 32));
	const auto& pConeMesh = std::make_shared<CMesh>(CPrimitive::createCone(0.4f, 1.0f, 32));
	const auto& pCylinderNode = std::make_shared<CNode>();
	const auto& pConeNode = std::make_shared<CNode>();
	pCylinderNode->addMesh(pCylinderMesh);
	pCylinderNode->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pConeNode->addMesh(pConeMesh);
	pConeNode->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	m_pArrowNode->addChild(pCylinderNode);
	m_pArrowNode->addChild(pConeNode);
}

void Elaina::CVisLightPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pLitFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pLitFrameBuffer->getWidth(), m_pLitFrameBuffer->getHeight()));
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
		m_pShaderProgram->setUniform("uLightColor", pLight->_LightColor);

		auto Model = glm::mat4(1.0f);
		Model = glm::translate(Model, pLight->_LightPos);
		if (pLight->getLightType() == ELightType::POINT)
		{
			Model = glm::scale(Model, glm::vec3(0.1f));
			m_pShaderProgram->setUniform("uModel", Model);
			m_pPointLightVAO->bind();
			m_pPointLightVAO->draw();
		}
		else if (pLight->getLightType() == ELightType::DIRECTIONAL)
		{
			glm::vec3 TargetDir = std::dynamic_pointer_cast<SDirectionalLight>(pLight)->_LightDir;
			glm::mat4 Rotation = __calcRotationMatrix(glm::vec3(0.0f, 1.0f, 0.0f), TargetDir);
			Model = Model * Rotation;
			Model = glm::scale(Model, glm::vec3(0.5f));
			CNode::traverse(m_pArrowNode, [&](const std::shared_ptr<CNode>& vNode)
			{
				m_pShaderProgram->setUniform("uModel", Model * vNode->getModelMatrix());
				for (const auto& pMesh : vNode->getMeshes()) {
					pMesh->draw();
				}
			});
		}
	}
}

bool Elaina::CVisLightPass::validateV() const
{
	if (m_pLitFrameBuffer == nullptr)
	{
		spdlog::error("[VisLightPass] Lit frame buffer miss");
		return false;
	}
	return true;
}

glm::mat4 Elaina::CVisLightPass::__calcRotationMatrix(const glm::vec3& vCurrDir, const glm::vec3& vTargetDir)
{
	const glm::vec3 CurrDir = glm::normalize(vCurrDir);
	const glm::vec3 TargetDir = glm::normalize(vTargetDir);

	const glm::vec3 RotationAxis = glm::cross(CurrDir, TargetDir);
	const float Angle = acos(glm::dot(CurrDir, TargetDir));
	return glm::rotate(glm::mat4(1.0f), Angle, RotationAxis);
}