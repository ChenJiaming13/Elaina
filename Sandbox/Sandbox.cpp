#include "pch.h"
#include "Sandbox.h"
#include "base/Framebuffer.h"
#include "core/Material.h"
#include "core/Mesh.h"
#include "core/Node.h"
#include "core/RenderPipeline.h"
#include "core/Scene.h"
#include "light/Light.h"
#include "loader/ModelLoader.h"
#include "primitive/Primitive.h"
#include "renderpass/DeferredGeoPass.h"
#include "renderpass/DeferredLitPass.h"
#include "renderpass/SkyBoxPass.h"
#include "renderpass/VisLightPass.h"
#include "renderpass/DirShadowMapPass.h"
#include "renderpass/ForwardLitPass.h"
#include "renderpass/PointShadowMapPass.h"
#include "renderpass/WaterLitPass.h"
#include "utils/AssetsPath.h"

void CSandbox::init(int vWidth, int vHeight)
{
	m_SkyBoxFiles = std::array{
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\right.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\left.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\top.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\bottom.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\front.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\back.jpg"
	};
	__setupScene(vWidth, vHeight);
	//__setupDeferredRenderPipeline(vWidth, vHeight);
	__setupForwardRenderPipeline(vWidth, vHeight);
}

void CSandbox::render(float vCurrTime, float vDeltaTime)
{
	m_pObjNode->clearChilds();
	m_pObjNode->addChild(m_pNodes[m_IndexOfNodes]);
	m_pObjNode->setRotation(glm::vec3(1.0f, 1.0f, 1.0f) * vCurrTime * 5.0f);
	m_pRenderPipeline->render(m_pScene);
	__renderUI();
}

void CSandbox::onWindowSizeChangeV(int vWidth, int vHeight)
{
	if (m_pRenderPipeline != nullptr) m_pRenderPipeline->onWindowSizeChange(vWidth, vHeight);
}

void CSandbox::__setupScene(int vWidth, int vHeight)
{
	m_pScene = std::make_shared<Elaina::CScene>();
	__setupMaterials();
	__setupNodes();
	__setupCamera(vWidth, vHeight);
	__setupLights();
}

void CSandbox::__setupDeferredRenderPipeline(int vWidth, int vHeight)
{
	m_IsDeferredPipeline = true;
	m_pDirShadowMapPass = std::make_shared<Elaina::CDirShadowMapPass>();
	m_pPointShadowMapPass = std::make_shared<Elaina::CPointShadowMapPass>();
	m_pDeferredLitPass = std::make_shared<Elaina::CDeferredLitPass>(true);
	const auto& pDeferredGeoPass = std::make_shared<Elaina::CDeferredGeoPass>();
	const auto& pSkyBoxPass = std::make_shared<Elaina::CSkyBoxPass>(m_SkyBoxFiles, true);
	const auto& pVisLightPass = std::make_shared<Elaina::CVisLightPass>();

	m_pRenderPipeline = std::make_shared<Elaina::CRenderPipeline>();
	m_pRenderPipeline->addRenderPass(m_pDirShadowMapPass);
	m_pRenderPipeline->addRenderPass(m_pPointShadowMapPass);
	m_pRenderPipeline->addRenderPass(pDeferredGeoPass);
	m_pRenderPipeline->addRenderPass(m_pDeferredLitPass);
	m_pRenderPipeline->addRenderPass(pSkyBoxPass);
	m_pRenderPipeline->addRenderPass(pVisLightPass);
	m_pRenderPipeline->init(vWidth, vHeight);

	m_pDeferredLitPass->setGeoPositionTex(pDeferredGeoPass->getGeoPositionTex());
	m_pDeferredLitPass->setGeoNormalTex(pDeferredGeoPass->getGeoNormalTex());
	m_pDeferredLitPass->setGeoDepthTex(pDeferredGeoPass->getGeoDepthTex());
	m_pDeferredLitPass->setGeoAlbedoTex(pDeferredGeoPass->getGeoAlbedoTex());
	m_pDeferredLitPass->setGeoPbrPropsTex(pDeferredGeoPass->getGeoPbrPropsTex());
	m_pDeferredLitPass->setDirShadowMapTex(m_pDirShadowMapPass->getShadowMap());
	m_pDeferredLitPass->setPointShadowMapTex(m_pPointShadowMapPass->getShadowMap());
	pSkyBoxPass->setGeoFrameBuffer(pDeferredGeoPass->getFrameBuffer());
	pSkyBoxPass->setLitFrameBuffer(m_pDeferredLitPass->getFrameBuffer());
	pVisLightPass->setLitFrameBuffer(m_pDeferredLitPass->getFrameBuffer());
}

void CSandbox::__setupForwardRenderPipeline(int vWidth, int vHeight)
{
	m_IsDeferredPipeline = false;
	const auto& pForwardLitPass = std::make_shared<Elaina::CForwardLitPass>(true);
	const auto& pSkyBoxPass = std::make_shared<Elaina::CSkyBoxPass>(m_SkyBoxFiles, false);
	const auto& pVisLightPass = std::make_shared<Elaina::CVisLightPass>();
	const auto& pWaterLitPass = std::make_shared<Elaina::CWaterLitPass>();
	m_pRenderPipeline = std::make_shared<Elaina::CRenderPipeline>();
	m_pRenderPipeline->addRenderPass(pForwardLitPass);
	m_pRenderPipeline->addRenderPass(pSkyBoxPass);
	m_pRenderPipeline->addRenderPass(pVisLightPass);
	m_pRenderPipeline->addRenderPass(pWaterLitPass);
	m_pRenderPipeline->init(vWidth, vHeight);

	pSkyBoxPass->setLitFrameBuffer(pForwardLitPass->getFrameBuffer());
	pVisLightPass->setLitFrameBuffer(pForwardLitPass->getFrameBuffer());
	pWaterLitPass->setLitFrameBuffer(pForwardLitPass->getFrameBuffer());
}

void CSandbox::__setupMaterials()
{
	m_pPlaneMat = std::make_shared<Elaina::SPbrMaterial>();
	m_pPlaneMat->_Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	m_pPlaneMat->_Metallic = 1.0f;
	m_pPlaneMat->_Roughness = 0.6f;
	m_pPlaneMat->_Ao = 1.0f;

	m_pObjMat = std::make_shared<Elaina::SPbrMaterial>();
	m_pObjMat->_Albedo = glm::vec3(1.0f, 0.0f, 0.0f);
	m_pObjMat->_Metallic = 0.5f;
	m_pObjMat->_Roughness = 1.0f;
	m_pObjMat->_Ao = 0.1f;

	m_pPhongMat = std::make_shared<Elaina::SPhongMaterial>();
	m_pPhongMat->_Color = glm::vec3(0.0f, 1.0f, 1.0f);
	m_pPhongMat->_Ambient = 0.2f;
	m_pPhongMat->_Specular = 0.5f;
	m_pPhongMat->_Glossy = 64.0f;

	m_pCheckerMat = std::make_shared<Elaina::SCheckerMaterial>();

	m_pWaterMat = std::make_shared<Elaina::SWaterMaterial>();
}

void CSandbox::__setupCamera(int vWidth, int vHeight)
{
	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, static_cast<float>(vWidth) / static_cast<float>(vHeight));
	m_pCameraController = std::make_shared<Elaina::CArcballController>();
	m_pCameraController->control(pCamera);
	m_pScene->setCamera(pCamera);
}

void CSandbox::__setupLights() const
{
	const auto& pDirLight = std::make_shared<Elaina::SDirectionalLight>();
	pDirLight->_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	pDirLight->_LightIntensity = 10.0f;
	pDirLight->_LightPos = glm::vec3(0.0f, 5.0f, 5.0f);
	pDirLight->_LightDir = glm::vec3(0.0f, 0.0f, 0.0f) - pDirLight->_LightPos;

	const auto& pPointLight = std::make_shared<Elaina::SPointLight>();
	pPointLight->_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	pPointLight->_LightIntensity = 1000.0f;
	pPointLight->_LightPos = glm::vec3(0.0f, 0.0f, 5.0f);

	m_pScene->setDirectionalLight(pDirLight);
	m_pScene->setPointLight(pPointLight);
}

void CSandbox::__setupNodes()
{
	const auto& pPlaneNode = __createNode(Elaina::CPrimitive::createPlane());
	const auto& pPlaneNode1 = __createNode(Elaina::CPrimitive::createPlane());
	const auto& pWaterPlaneNode = __createNode(Elaina::CPrimitive::createPlane());
	const auto& pCubeNode = __createNode(Elaina::CPrimitive::createCube());
	//const auto& pPlaneNode2 = createNode(Elaina::CPrimitive::createPlane());
	pPlaneNode->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	pPlaneNode1->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	pWaterPlaneNode->setScale(glm::vec3(8.0f, 1.0f, 8.0f));
	//pPlaneNode2->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	pPlaneNode->setPosition(glm::vec3(0.0f, -5.0f, 0.0f));
	pPlaneNode1->setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	pPlaneNode1->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
	pWaterPlaneNode->setPosition(glm::vec3(0.0f, -3.0f, 0.0f));
	pCubeNode->setPosition(glm::vec3(0.0f, -4.0f, 0.0f));
	__setMaterial(pPlaneNode, m_pPlaneMat);
	__setMaterial(pPlaneNode1, m_pPlaneMat);
	__setMaterial(pCubeNode, m_pObjMat);
	__setMaterial(pWaterPlaneNode, m_pWaterMat);
	const auto& pRootNode = std::make_shared<Elaina::CNode>();
	m_pObjNode = std::make_shared<Elaina::CNode>();
	pRootNode->addChild(pPlaneNode);
	pRootNode->addChild(pPlaneNode1);
	//pRootNode->addChild(pWaterPlaneNode);
	pRootNode->addChild(pCubeNode);
	pRootNode->addChild(m_pObjNode);

	m_pNodes = {
		__createNode(Elaina::CPrimitive::createTorus()),
		__createNode(Elaina::CPrimitive::createCube()),
		__createNode(Elaina::CPrimitive::createSphere()),
		Elaina::CModelLoader::loadGltfFile(Elaina::CAssetsPath::getAssetsPath() + "trimesh/dragon.gltf")
	};
	for (const auto& pNode : m_pNodes) __setMaterial(pNode, m_pObjMat);
	m_pScene->setRootNode(pRootNode);
}

void CSandbox::__renderUI()
{
	int ID = 0;
	ImGui::Begin("Inspector");
	if (m_IsDeferredPipeline)
	{
		if (ImGui::CollapsingHeader("Render Pipeline"))
		{
			ImGui::PushID(ID++);
			static bool EnableSkyBoxPass;
			constexpr size_t IdxOfSkyBoxPass = 4;
			EnableSkyBoxPass = m_pRenderPipeline->getPassEnable(IdxOfSkyBoxPass);
			if (ImGui::Checkbox("Enable SkyBox Pass", &EnableSkyBoxPass))
			{
				m_pRenderPipeline->setPassEnable(IdxOfSkyBoxPass, EnableSkyBoxPass);
			}
			ImGui::PopID();
		}
	}
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::PushID(ID++);
		static int DirShadowMapSize[] = { 0, 0 };
		m_pDirShadowMapPass->getShadowMapSize(DirShadowMapSize[0], DirShadowMapSize[1]);
		ImGui::ColorEdit3("Light Color", &m_pScene->getDirectionalLight()->_LightColor.x);
		ImGui::DragFloat("Light Intensity", &m_pScene->getDirectionalLight()->_LightIntensity, 0.01f);
		ImGui::DragFloat3("Light Direction", &m_pScene->getDirectionalLight()->_LightDir.x, 0.01f);
		ImGui::DragFloat3("Light Position", &m_pScene->getDirectionalLight()->_LightPos.x, 0.01f);
		if (ImGui::DragInt2("Shadow Map Size", DirShadowMapSize, 5.0f))
		{
			if (DirShadowMapSize[0] > 0 && DirShadowMapSize[1] > 0)
			{
				m_pDirShadowMapPass->setShadowMapSize(DirShadowMapSize[0], DirShadowMapSize[1]);
			}
		}
		static bool EnablePCF;
		static int HalfPCFSize;
		EnablePCF = m_pDeferredLitPass->getEnablePCF();
		HalfPCFSize = m_pDeferredLitPass->getHalfSizePCF();
		if (ImGui::Checkbox("Enable PCF", &EnablePCF))
		{
			m_pDeferredLitPass->setEnablePCF(EnablePCF);
		}
		if (ImGui::DragInt("Half PCF Size", &HalfPCFSize, 1, 0, 5))
		{
			m_pDeferredLitPass->setHalfSizePCF(HalfPCFSize);
		}
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Point Light"))
	{
		ImGui::PushID(ID++);
		static int PointShadowMapSize = 0;
		m_pPointShadowMapPass->setShadowMapSize(PointShadowMapSize, PointShadowMapSize);
		ImGui::ColorEdit3("Light Color", &m_pScene->getPointLight()->_LightColor.x);
		ImGui::DragFloat("Light Intensity", &m_pScene->getPointLight()->_LightIntensity, 0.01f);
		ImGui::DragFloat3("Light Position", &m_pScene->getPointLight()->_LightPos.x, 0.01f);
		if (ImGui::DragInt("Shadow Map Size", &PointShadowMapSize, 5.0f))
		{
			if (PointShadowMapSize > 0)
			{
				m_pPointShadowMapPass->setShadowMapSize(PointShadowMapSize, PointShadowMapSize);
			}
		}
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Obj PBR Material"))
	{
		ImGui::PushID(ID++);
		ImGui::ColorEdit3("Albedo", &m_pObjMat->_Albedo.x);
		ImGui::DragFloat("Metallic", &m_pObjMat->_Metallic, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Roughness", &m_pObjMat->_Roughness, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ao", &m_pObjMat->_Ao, 0.01f, 0.0f, 1.0f);
		if (ImGui::Button("Switch Model"))
		{
			m_IndexOfNodes = (m_IndexOfNodes + 1) % m_pNodes.size();
		}
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Plane PBR Material"))
	{
		ImGui::PushID(ID);
		ImGui::ColorEdit3("Albedo", &m_pPlaneMat->_Albedo.x);
		ImGui::DragFloat("Metallic", &m_pPlaneMat->_Metallic, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Roughness", &m_pPlaneMat->_Roughness, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ao", &m_pPlaneMat->_Ao, 0.01f, 0.0f, 1.0f);
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Phong Material"))
	{
		ImGui::PushID(ID);
		ImGui::ColorEdit3("Color", &m_pPhongMat->_Color.x);
		ImGui::DragFloat("Ambient", &m_pPhongMat->_Ambient, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Specular", &m_pPhongMat->_Specular, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Glossy", &m_pPhongMat->_Glossy, 1.0f, 0.0f, 128.0f);
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Checker Material"))
	{
		ImGui::PushID(ID);
		ImGui::ColorEdit3("PrimaryColor", &m_pCheckerMat->_PrimaryColor.x);
		ImGui::ColorEdit3("SecondaryColor", &m_pCheckerMat->_SecondaryColor.x);
		ImGui::DragFloat("Scale", &m_pCheckerMat->_Scale, 0.1f, 0.0f, 10.0f);
		ImGui::PopID();
	}
	ImGui::End();
}

std::shared_ptr<Elaina::CNode> CSandbox::__createNode(const std::shared_ptr<Elaina::CVertexArrayObject>& vVAO)
{
	const auto& pNode = std::make_shared<Elaina::CNode>();
	pNode->addMesh(std::make_shared<Elaina::CMesh>(vVAO));
	return pNode;
}

void CSandbox::__setMaterial(const std::shared_ptr<Elaina::CNode>& vRootNode, const std::shared_ptr<Elaina::SMaterial>& vMaterial)
{
	Elaina::CNode::traverse(vRootNode, [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		for (const auto& pMesh : vNode->getMeshes())
			pMesh->setMaterial(vMaterial);
		});
}