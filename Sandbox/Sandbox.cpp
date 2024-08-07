#include "pch.h"
#include "Sandbox.h"
#include "base/Framebuffer.h"
#include "base/TextureCube.h"
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
#include "renderpass/DeferredSkyBoxPass.h"
#include "renderpass/DeferVisLightPass.h"
#include "renderpass/DirShadowMapPass.h"
#include "renderpass/ForwardLitPass.h"
#include "renderpass/PointShadowMapPass.h"
#include "utils/AssetsPath.h"
#include "utils/FrameBufferHelper.h"

void CSandbox::init(int vWidth, int vHeight)
{
	__setupScene(vWidth, vHeight);
	//__setupDeferredRenderPipeline(vWidth, vHeight);
	__setupForwardRenderPipeline(vWidth, vHeight);
}

void CSandbox::render(float vCurrTime, float vDeltaTime)
{
	m_pObjNode->clearChilds();
	m_pObjNode->addChild(m_pNodes[m_IndexOfNodes]);
	m_pObjNode->setRotation(glm::vec3(1.0f, 1.0f, 1.0f) * vCurrTime * 5.0f);
	m_RenderPipeline->render(m_Scene);
	__renderUI();
}

void CSandbox::onWindowSizeChange(int vWidth, int vHeight)
{
	if (m_RenderPipeline != nullptr) m_RenderPipeline->resize(vWidth, vHeight);
}

void CSandbox::__setupScene(int vWidth, int vHeight)
{
	m_Scene = std::make_shared<Elaina::CScene>();
	__setupMaterials();
	__setupNodes();
	__setupCamera(vWidth, vHeight);
	__setupLights();
}

void CSandbox::__setupDeferredRenderPipeline(int vWidth, int vHeight)
{
	m_IsDeferredPipeline = true;
	const auto& pDirShadowMapPass = std::make_shared<Elaina::CDirShadowMapPass>();
	const auto& pPointShadowMapPass = std::make_shared<Elaina::CPointShadowMapPass>();
	const auto& pDeferredGeoPass = std::make_shared<Elaina::CDeferredGeoPass>();
	m_DeferredLitPass = std::make_shared<Elaina::CDeferredLitPass>(2, 0, 1, pDirShadowMapPass);
	const auto& pSkyBoxTex = std::make_shared<Elaina::CTextureCube>(std::array{
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\right.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\left.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\top.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\bottom.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\front.jpg",
		Elaina::CAssetsPath::getAssetsPath() + "skybox\\back.jpg"
	});
	pSkyBoxTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	pSkyBoxTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	pSkyBoxTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	pSkyBoxTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	pSkyBoxTex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	const auto& pDeferredSkyBoxPass = std::make_shared<Elaina::CDeferredSkyBoxPass>(pSkyBoxTex, 2);

	const auto& pVisLightPass = std::make_shared<Elaina::CDeferVisLightPass>();

	m_RenderPipeline = std::make_shared<Elaina::CRenderPipeline>();
	m_DirShadowMapFB = Elaina::CFrameBufferHelper::createDepthOnlyFrameBuffer(1024, 1024);
	m_PointShadowMapFB = Elaina::CFrameBufferHelper::createPointLightShadowFrameBuffer(512, 512);
	m_RenderPipeline->addFrameBuffer(m_DirShadowMapFB);
	m_RenderPipeline->addFrameBuffer(m_PointShadowMapFB);
	m_RenderPipeline->addFrameBuffer(Elaina::CFrameBufferHelper::createColorAndDepthFrameBuffer(vWidth, vHeight, std::vector<int>(4, 3)));
	m_RenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::getDefaultFrameBuffer());
	m_RenderPipeline->addRenderPass(pDirShadowMapPass, 0, false);
	m_RenderPipeline->addRenderPass(pPointShadowMapPass, 1, false);
	m_RenderPipeline->addRenderPass(pDeferredGeoPass, 2);
	m_RenderPipeline->addRenderPass(m_DeferredLitPass, 3);
	m_RenderPipeline->addRenderPass(pDeferredSkyBoxPass, 3);
	m_RenderPipeline->addRenderPass(pVisLightPass, 3);
}

void CSandbox::__setupForwardRenderPipeline(int vWidth, int vHeight)
{
	m_IsDeferredPipeline = false;
	const auto& pForwardLitPass = std::make_shared<Elaina::CForwardLitPass>();
	m_RenderPipeline = std::make_shared<Elaina::CRenderPipeline>();
	m_RenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::getDefaultFrameBuffer());
	m_RenderPipeline->addRenderPass(pForwardLitPass, 0);
}

void CSandbox::__setupMaterials()
{
	m_PlaneMat = std::make_shared<Elaina::SPbrMaterial>();
	m_PlaneMat->_Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	m_PlaneMat->_Metallic = 1.0f;
	m_PlaneMat->_Roughness = 0.6f;
	m_PlaneMat->_Ao = 1.0f;

	m_ObjMat = std::make_shared<Elaina::SPbrMaterial>();
	m_ObjMat->_Albedo = glm::vec3(1.0f, 0.0f, 0.0f);
	m_ObjMat->_Metallic = 0.5f;
	m_ObjMat->_Roughness = 1.0f;
	m_ObjMat->_Ao = 0.1f;

	m_PhongMat = std::make_shared<Elaina::SPhongMaterial>();
	m_PhongMat->_Color = glm::vec3(0.0f, 1.0f, 1.0f);
	m_PhongMat->_Ambient = 0.2f;
	m_PhongMat->_Specular = 0.5f;
	m_PhongMat->_Glossy = 64.0f;

	m_CheckerMat = std::make_shared<Elaina::SCheckerMaterial>();
}

void CSandbox::__setupCamera(int vWidth, int vHeight)
{
	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, static_cast<float>(vWidth) / static_cast<float>(vHeight));
	m_pCameraController = std::make_shared<Elaina::CArcballController>();
	m_pCameraController->control(pCamera);
	m_Scene->setCamera(pCamera);
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

	m_Scene->setDirectionalLight(pDirLight);
	m_Scene->setPointLight(pPointLight);
}

void CSandbox::__setupNodes()
{
	const auto& pPlaneNode = __createNode(Elaina::CPrimitive::createPlane());
	const auto& pPlaneNode1 = __createNode(Elaina::CPrimitive::createPlane());
	const auto& pCubeNode = __createNode(Elaina::CPrimitive::createCube());
	//const auto& pPlaneNode2 = createNode(Elaina::CPrimitive::createPlane());
	pPlaneNode->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	pPlaneNode1->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	//pPlaneNode2->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	pPlaneNode->setPosition(glm::vec3(0.0f, -5.0f, 0.0f));
	pPlaneNode1->setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	pPlaneNode1->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
	pCubeNode->setPosition(glm::vec3(0.0f, -4.0f, 0.0f));
	__setMaterial(pPlaneNode, m_CheckerMat);
	__setMaterial(pPlaneNode1, m_PlaneMat);
	__setMaterial(pCubeNode, m_PhongMat);
	const auto& pRootNode = std::make_shared<Elaina::CNode>();
	m_pObjNode = std::make_shared<Elaina::CNode>();
	pRootNode->addChild(pPlaneNode);
	pRootNode->addChild(pPlaneNode1);
	pRootNode->addChild(pCubeNode);
	pRootNode->addChild(m_pObjNode);

	m_pNodes = {
		__createNode(Elaina::CPrimitive::createTorus()),
		__createNode(Elaina::CPrimitive::createCube()),
		__createNode(Elaina::CPrimitive::createSphere()),
		Elaina::CModelLoader::loadGltfFile(Elaina::CAssetsPath::getAssetsPath() + "trimesh/dragon.gltf")
	};
	for (const auto& pNode : m_pNodes) __setMaterial(pNode, m_ObjMat);
	m_Scene->setRootNode(pRootNode);
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
			EnableSkyBoxPass = m_RenderPipeline->getPassEnable(IdxOfSkyBoxPass);
			if (ImGui::Checkbox("Enable SkyBox Pass", &EnableSkyBoxPass))
			{
				m_RenderPipeline->setPassEnable(IdxOfSkyBoxPass, EnableSkyBoxPass);
			}
			ImGui::PopID();
		}
		if (ImGui::CollapsingHeader("Directional Light"))
		{
			ImGui::PushID(ID++);
			static int DirShadowMapSize[] = { 0, 0 };
			DirShadowMapSize[0] = m_DirShadowMapFB->getWidth();
			DirShadowMapSize[1] = m_DirShadowMapFB->getHeight();
			ImGui::ColorEdit3("Light Color", &m_Scene->getDirectionalLight()->_LightColor.x);
			ImGui::DragFloat("Light Intensity", &m_Scene->getDirectionalLight()->_LightIntensity, 0.01f);
			ImGui::DragFloat3("Light Direction", &m_Scene->getDirectionalLight()->_LightDir.x, 0.01f);
			ImGui::DragFloat3("Light Position", &m_Scene->getDirectionalLight()->_LightPos.x, 0.01f);
			if (ImGui::DragInt2("Shadow Map Size", DirShadowMapSize, 5.0f))
			{
				if (DirShadowMapSize[0] > 0 && DirShadowMapSize[1] > 0)
				{
					m_DirShadowMapFB->resize(DirShadowMapSize[0], DirShadowMapSize[1]);
				}
			}
			static bool EnablePCF;
			static int HalfPCFSize;
			EnablePCF = m_DeferredLitPass->getEnablePCF();
			HalfPCFSize = m_DeferredLitPass->getHalfSizePCF();
			if (ImGui::Checkbox("Enable PCF", &EnablePCF))
			{
				m_DeferredLitPass->setEnablePCF(EnablePCF);
			}
			if (ImGui::DragInt("Half PCF Size", &HalfPCFSize, 1, 0, 5))
			{
				m_DeferredLitPass->setHalfSizePCF(HalfPCFSize);
			}
			ImGui::PopID();
		}
		if (ImGui::CollapsingHeader("Point Light"))
		{
			ImGui::PushID(ID++);
			static int PointShadowMapSize = 0;
			PointShadowMapSize = m_PointShadowMapFB->getWidth();
			ImGui::ColorEdit3("Light Color", &m_Scene->getPointLight()->_LightColor.x);
			ImGui::DragFloat("Light Intensity", &m_Scene->getPointLight()->_LightIntensity, 0.01f);
			ImGui::DragFloat3("Light Position", &m_Scene->getPointLight()->_LightPos.x, 0.01f);
			if (ImGui::DragInt("Shadow Map Size", &PointShadowMapSize, 5.0f))
			{
				if (PointShadowMapSize > 0)
				{
					m_PointShadowMapFB->resize(PointShadowMapSize, PointShadowMapSize);
				}
			}
			ImGui::PopID();
		}
	}
	if (ImGui::CollapsingHeader("Obj PBR Material"))
	{
		ImGui::PushID(ID++);
		ImGui::ColorEdit3("Albedo", &m_ObjMat->_Albedo.x);
		ImGui::DragFloat("Metallic", &m_ObjMat->_Metallic, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Roughness", &m_ObjMat->_Roughness, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ao", &m_ObjMat->_Ao, 0.01f, 0.0f, 1.0f);
		if (ImGui::Button("Switch Model"))
		{
			m_IndexOfNodes = (m_IndexOfNodes + 1) % m_pNodes.size();
		}
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Plane PBR Material"))
	{
		ImGui::PushID(ID);
		ImGui::ColorEdit3("Albedo", &m_PlaneMat->_Albedo.x);
		ImGui::DragFloat("Metallic", &m_PlaneMat->_Metallic, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Roughness", &m_PlaneMat->_Roughness, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ao", &m_PlaneMat->_Ao, 0.01f, 0.0f, 1.0f);
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Phong Material"))
	{
		ImGui::PushID(ID);
		ImGui::ColorEdit3("Color", &m_PhongMat->_Color.x);
		ImGui::DragFloat("Ambient", &m_PhongMat->_Ambient, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Specular", &m_PhongMat->_Specular, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Glossy", &m_PhongMat->_Glossy, 1.0f, 0.0f, 128.0f);
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Checker Material"))
	{
		ImGui::PushID(ID);
		ImGui::ColorEdit3("PrimaryColor", &m_CheckerMat->_PrimaryColor.x);
		ImGui::ColorEdit3("SecondaryColor", &m_CheckerMat->_SecondaryColor.x);
		ImGui::DragFloat("Scale", &m_CheckerMat->_Scale, 0.1f, 0.0f, 10.0f);
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