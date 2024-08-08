#include "pch.h"
#include "ForwardLitPass.h"
#include "base/ShaderProgram.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "core/Mesh.h"
#include "core/Material.h"
#include "core/Camera.h"
#include "light/Light.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "utils/AssetsPath.h"

Elaina::CForwardLitPass::CForwardLitPass(bool vIsFinalPass):
	m_pShaderProgramPBR(CShaderProgram::createShaderProgram(
		CAssetsPath::getAssetsPath() + "shaders/forwardPbr.vert",
		CAssetsPath::getAssetsPath() + "shaders/forwardPbr.frag"
	)),
	m_pShaderProgramPhong(CShaderProgram::createShaderProgram(
		CAssetsPath::getAssetsPath() + "shaders/forwardPhong.vert",
		CAssetsPath::getAssetsPath() + "shaders/forwardPhong.frag"
	)),
	m_pShaderProgramChecker(CShaderProgram::createShaderProgram(
		CAssetsPath::getAssetsPath() + "shaders/forwardChecker.vert",
		CAssetsPath::getAssetsPath() + "shaders/forwardChecker.frag"
	)),
	m_IsFinalPass(vIsFinalPass)
{}

Elaina::CForwardLitPass::~CForwardLitPass()
{
	m_pShaderProgramPBR.reset();
	m_pShaderProgramPhong.reset();
	m_pShaderProgramPhong.reset();
}

void Elaina::CForwardLitPass::initV(int vWidth, int vHeight)
{
	if (m_IsFinalPass)
		m_pFrameBuffer = CFrameBuffer::getDefaultFrameBuffer();
	else
		m_pFrameBuffer = CFrameBuffer::createFrameBuffer(vWidth, vHeight, std::vector{ 3 });
}

void Elaina::CForwardLitPass::onWindowSizeChangeV(int vWidth, int vHeight)
{
	m_pFrameBuffer->resize(vWidth, vHeight);
}

void Elaina::CForwardLitPass::renderV(const std::shared_ptr<CScene>& vScene)
{
	m_pFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pFrameBuffer->getWidth(), m_pFrameBuffer->getHeight()));
	const auto& pCamera = vScene->getCamera();
	const auto& pDirLight = vScene->getDirectionalLight();
	const glm::vec4 SolidColor = pCamera->getSolidColor();
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClearColor(SolidColor.x, SolidColor.y, SolidColor.z, SolidColor.w));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	m_pShaderProgramPBR->use();
	m_pShaderProgramPBR->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgramPBR->setUniform("uProjection", pCamera->getProjectionMatrix());
	m_pShaderProgramPBR->setUniform("uCamPos", pCamera->getWorldPos());
	m_pShaderProgramPBR->setUniform("uLightDir", pDirLight->_LightDir);
	m_pShaderProgramPBR->setUniform("uLightColor", pDirLight->_LightColor * pDirLight->_LightIntensity);
	
	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		m_pShaderProgramPBR->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			if (pMesh->getMaterial()->getMaterialType() != EMaterialType::PBR)
				continue;
			const auto& pMaterial = std::dynamic_pointer_cast<SPbrMaterial>(pMesh->getMaterial());
			m_pShaderProgramPBR->setUniform("uAlbedo", pMaterial->_Albedo);
			m_pShaderProgramPBR->setUniform("uMetallic", pMaterial->_Metallic);
			m_pShaderProgramPBR->setUniform("uRoughness", pMaterial->_Roughness);
			m_pShaderProgramPBR->setUniform("uAo", pMaterial->_Ao);
			pMesh->draw();
		}
	});

	m_pShaderProgramPhong->use();
	m_pShaderProgramPhong->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgramPhong->setUniform("uProjection", pCamera->getProjectionMatrix());
	m_pShaderProgramPhong->setUniform("uViewPos", pCamera->getWorldPos());
	m_pShaderProgramPhong->setUniform("uLightDir", pDirLight->_LightDir);
	m_pShaderProgramPhong->setUniform("uLightColor", pDirLight->_LightColor);

	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		m_pShaderProgramPhong->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			if (pMesh->getMaterial()->getMaterialType() != EMaterialType::PHONG)
				continue;
			const auto& pMaterial = std::dynamic_pointer_cast<SPhongMaterial>(pMesh->getMaterial());
			m_pShaderProgramPhong->setUniform("uColor", pMaterial->_Color);
			m_pShaderProgramPhong->setUniform("uAmbient", pMaterial->_Ambient);
			m_pShaderProgramPhong->setUniform("uSpecular", pMaterial->_Specular);
			m_pShaderProgramPhong->setUniform("uGlossy", pMaterial->_Glossy);
			pMesh->draw();
		}
	});

	m_pShaderProgramChecker->use();
	m_pShaderProgramChecker->setUniform("uView", pCamera->getViewMatrix());
	m_pShaderProgramChecker->setUniform("uProjection", pCamera->getProjectionMatrix());

	CNode::traverse(vScene->getRootNode(), [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		m_pShaderProgramChecker->setUniform("uModel", vNode->getModelMatrix());
		for (const auto& pMesh : vNode->getMeshes())
		{
			if (pMesh->getMaterial()->getMaterialType() != EMaterialType::CHECKER)
				continue;
			const auto& pMaterial = std::dynamic_pointer_cast<SCheckerMaterial>(pMesh->getMaterial());
			m_pShaderProgramChecker->setUniform("uPrimaryColor", pMaterial->_PrimaryColor);
			m_pShaderProgramChecker->setUniform("uSecondaryColor", pMaterial->_SecondaryColor);
			m_pShaderProgramChecker->setUniform("uScale", pMaterial->_Scale);
			pMesh->draw();
		}
	});
}
