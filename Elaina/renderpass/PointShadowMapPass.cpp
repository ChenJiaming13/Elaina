#include "pch.h"
#include "PointShadowMapPass.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/TextureCube.h"
#include "scene/Mesh.h"
#include "scene/Node.h"
#include "scene/Scene.h"
#include "light/Light.h"
#include "utils/AssetsPath.h"

Elaina::CPointShadowMapPass::CPointShadowMapPass() :m_pShaderProgram(CShaderProgram::createShaderProgram(
	CAssetsPath::getAssetsPath() + "shaders\\shadowMapPoint.vert",
	CAssetsPath::getAssetsPath() + "shaders\\shadowMapPoint.frag",
	CAssetsPath::getAssetsPath() + "shaders\\shadowMapPoint.geom"
)) {}

void Elaina::CPointShadowMapPass::initV(int vWidth, int vHeight)
{
	const auto& pDepthTex = std::make_shared<CTextureCube>(512, 512, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	pDepthTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	m_pFrameBuffer = std::make_shared<CFrameBuffer>();
	m_pFrameBuffer->create();
	m_pFrameBuffer->bind();
	m_pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex, 0);
	CFrameBuffer::setColorBufferEmpty();
	CFrameBuffer::checkComplete();
	CFrameBuffer::unbind();
}

void Elaina::CPointShadowMapPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pFrameBuffer->getWidth(), m_pFrameBuffer->getHeight()));
	m_Aspect = static_cast<float>(m_pFrameBuffer->getWidth()) / static_cast<float>(m_pFrameBuffer->getHeight());
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

void Elaina::CPointShadowMapPass::getShadowMapSize(int& voWidth, int& voHeight) const
{
	voWidth = m_pFrameBuffer->getWidth();
	voHeight = m_pFrameBuffer->getHeight();
}

void Elaina::CPointShadowMapPass::setShadowMapSize(int vWidth, int vHeight) const
{
	m_pFrameBuffer->resize(vWidth, vHeight);
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
