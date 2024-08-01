#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "base/Framebuffer.h"
#include "base/TextureCube.h"
#include "core/GlfwWindow.h"
#include "core/Camera.h"
#include "core/Mesh.h"
#include "core/Material.h"
#include "core/Node.h"
#include "core/Scene.h"
#include "core/RenderPipeline.h"
#include "core/RenderPass.h"
#include "renderpass/ForwardPbrPass.h"
#include "renderpass/PostProcessPass.h"
#include "renderpass/DeferredGeoPass.h"
#include "renderpass/DeferredLitPass.h"
#include "renderpass/DirShadowMapPass.h"
#include "renderpass/DeferredSkyBoxPass.h"
#include "light/Light.h"
#include "controller/ArcballController.h"
#include "primitive/Primitive.h"
#include "loader/ModelLoader.h"
#include "ui/ImGui.h"

size_t g_IndexOfNodes = 0;
size_t g_SizeofNodes = 0;
std::shared_ptr<Elaina::CScene> g_Scene = nullptr;
std::shared_ptr<Elaina::CRenderPipeline> g_RenderPipeline = nullptr;
std::shared_ptr<Elaina::SPbrMaterial> g_PlaneMat = nullptr;
std::shared_ptr<Elaina::SPbrMaterial> g_ObjMat = nullptr;
std::shared_ptr<Elaina::CFrameBuffer> g_DirShadowMapFB = nullptr;

class CMyInputHandler : public Elaina::CInputHandler
{
public:
	void onWindowSizeChange(int vWidth, int vHeight) override
	{
		if (g_RenderPipeline != nullptr) g_RenderPipeline->resize(vWidth, vHeight);
	}

	void onKeyDown(int vKey) override
	{
		if (vKey == 'X')
		{
			g_IndexOfNodes = (g_IndexOfNodes + 1) % g_SizeofNodes;
		}
	}
};

std::shared_ptr<Elaina::CNode> createNode(const std::shared_ptr<Elaina::CVertexArrayObject>& vVAO)
{
	const auto& pNode = std::make_shared<Elaina::CNode>();
	pNode->addMesh(std::make_shared<Elaina::CMesh>(vVAO));
	return pNode;
}

void setMaterial(const std::shared_ptr<Elaina::CNode>& vRootNode, const std::shared_ptr<Elaina::SMaterial>& vMaterial)
{
	Elaina::CNode::traverse(vRootNode, [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		for (const auto& pMesh : vNode->getMeshes())
			pMesh->setMaterial(vMaterial);
	});
}

void setRenderPipeline(int vWidth, int vHeight)
{
	const auto& pDirShadowMapPass = std::make_shared<Elaina::CDirShadowMapPass>(Elaina::CShaderProgram::createShaderProgram(
		"shaders\\dirShadowMap.vert",
		"shaders\\dirShadowMap.frag"
	));
	const auto& pDeferredGeoPass = std::make_shared<Elaina::CDeferredGeoPass>(Elaina::CShaderProgram::createShaderProgram(
		"shaders\\deferGeo.vert",
		"shaders\\deferGeo.frag"
	));
	const auto& pDeferredLitPass = std::make_shared<Elaina::CDeferredLitPass>(Elaina::CShaderProgram::createShaderProgram(
		"shaders\\deferPbr.vert",
		"shaders\\deferPbr.frag"
	), 1, 0, pDirShadowMapPass);
	
	const auto& pSkyBoxTex = std::make_shared<Elaina::CTextureCube>();
	pSkyBoxTex->loadCubeMapFiles(std::vector<std::string>{
		"skybox\\right.jpg",
		"skybox\\left.jpg",
		"skybox\\top.jpg",
		"skybox\\bottom.jpg",
		"skybox\\front.jpg",
		"skybox\\back.jpg"
	});
	const auto& pDeferredSkyBoxPass = std::make_shared<Elaina::CDeferredSkyBoxPass>(Elaina::CShaderProgram::createShaderProgram(
		"shaders\\deferSkyBox.vert",
		"shaders\\deferSkyBox.frag"
	), pSkyBoxTex, 1);

	g_RenderPipeline = std::make_shared<Elaina::CRenderPipeline>();
	g_DirShadowMapFB = Elaina::CFrameBuffer::createDepthOnlyFrameBuffer(1024, 1024);
	g_RenderPipeline->addFrameBuffer(g_DirShadowMapFB);
	g_RenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::createFrameBuffer(vWidth, vHeight, 4, true, false));
	g_RenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::getDefaultFrameBuffer());
	g_RenderPipeline->addRenderPass(pDirShadowMapPass, 0, false);
	g_RenderPipeline->addRenderPass(pDeferredGeoPass, 1);
	g_RenderPipeline->addRenderPass(pDeferredLitPass, 2);
	g_RenderPipeline->addRenderPass(pDeferredSkyBoxPass, 2);
	//g_RenderPipeline->addRenderPass(std::make_shared<Elaina::CForwardPbrPass>(Elaina::CShaderProgram::createShaderProgram(
	//	"shaders\\pbr.vert", 
	//	"shaders\\pbr.frag"
	//)), 2);
	//g_RenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(Elaina::CShaderProgram::createShaderProgram(
	//	"shaders\\postProcess.vert",
	//	"shaders\\postProcessInversion.frag"
	//)), 1);
	//g_RenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(Elaina::CShaderProgram::createShaderProgram(
	//	"shaders\\postProcess.vert",
	//	"shaders\\postProcessInversion.frag"
	//)), 0);
	//g_RenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(Elaina::CShaderProgram::createShaderProgram(
	//	"shaders\\postProcess.vert",
	//	"shaders\\postProcessInversion.frag"
	//)), 1);
	//g_RenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(Elaina::CShaderProgram::createShaderProgram(
	//	"shaders\\postProcess.vert",
	//	"shaders\\postProcessInversion.frag"
	//)), 2);
}

void renderUI()
{
	int ID = 0;
	ImGui::Begin("Inspector");
	if (ImGui::CollapsingHeader("Render Pipeline"))
	{
		ImGui::PushID(ID++);
		static bool EnableSkyBoxPass;
		size_t IdxOfSkyBoxPass = 3;
		EnableSkyBoxPass = g_RenderPipeline->getPassEnable(IdxOfSkyBoxPass);
		if (ImGui::Checkbox("Enable SkyBox Pass", &EnableSkyBoxPass))
		{
			g_RenderPipeline->setPassEnable(IdxOfSkyBoxPass, EnableSkyBoxPass);
		}
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::PushID(ID++);
		static int ShadowMapSize[] = { 0, 0 };
		ShadowMapSize[0] = g_DirShadowMapFB->getWidth();
		ShadowMapSize[1] = g_DirShadowMapFB->getHeight();
		ImGui::ColorEdit3("Light Color", &g_Scene->getDirectionalLight()->_LightColor.x);
		ImGui::DragFloat("Light Intensity", &g_Scene->getDirectionalLight()->_LightIntensity, 0.01f);
		ImGui::DragFloat3("Light Direction", &g_Scene->getDirectionalLight()->_LightDir.x, 0.01f);
		ImGui::DragFloat3("Light Position", &g_Scene->getDirectionalLight()->_LightPos.x, 0.01f);
		if (ImGui::DragInt2("Shadow Map Size", ShadowMapSize, 5.0f))
		{
			if (ShadowMapSize[0] > 0 && ShadowMapSize[1] > 0)
			{
				g_DirShadowMapFB->resize(ShadowMapSize[0], ShadowMapSize[1]);
			}
		}
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Obj PBR Material"))
	{
		ImGui::PushID(ID++);
		ImGui::ColorEdit3("Albedo", &g_ObjMat->_Albedo.x);
		ImGui::DragFloat("Metallic", &g_ObjMat->_Metallic, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Roughness", &g_ObjMat->_Roughness, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ao", &g_ObjMat->_Ao, 0.01f, 0.0f, 1.0f);
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Plane PBR Material"))
	{
		ImGui::PushID(ID++);
		ImGui::ColorEdit3("Albedo", &g_PlaneMat->_Albedo.x);
		ImGui::DragFloat("Metallic", &g_PlaneMat->_Metallic, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Roughness", &g_PlaneMat->_Roughness, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Ao", &g_PlaneMat->_Ao, 0.01f, 0.0f, 1.0f);
		ImGui::PopID();
	}
	ImGui::End();
}

int main()
{
	Elaina::CGlfwWindow App;
	_ASSERTE(App.init(800, 600));
	Elaina::CFrameBuffer::initDefaultFrameBuffer(App.getWidth(), App.getHeight(), 0);

	g_PlaneMat = std::make_shared<Elaina::SPbrMaterial>();
	g_PlaneMat->_Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	g_PlaneMat->_Metallic = 0.5f;
	g_PlaneMat->_Roughness = 0.1f;
	g_PlaneMat->_Ao = 0.1f;

	g_ObjMat = std::make_shared<Elaina::SPbrMaterial>();
	g_ObjMat->_Albedo = glm::vec3(1.0f, 0.0f, 0.0f);
	g_ObjMat->_Metallic = 0.5f;
	g_ObjMat->_Roughness = 1.0f;
	g_ObjMat->_Ao = 0.1f;

	const auto& pPlaneNode = createNode(Elaina::CPrimitive::createPlane());
	pPlaneNode->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	pPlaneNode->setPosition(glm::vec3(0.0f, -5.0f, 0.0f));
	setMaterial(pPlaneNode, g_PlaneMat);
	const auto& pRootNode = std::make_shared<Elaina::CNode>();
	const auto& pTestNode = std::make_shared<Elaina::CNode>();
	pRootNode->addChild(pPlaneNode);
	pRootNode->addChild(pTestNode);

	std::vector<std::shared_ptr<Elaina::CNode>> Nodes{
		//createNode(Elaina::CPrimitive::createQuad()),
		createNode(Elaina::CPrimitive::createTorus()),
		createNode(Elaina::CPrimitive::createCube()),
		createNode(Elaina::CPrimitive::createSphere()),
		Elaina::CModelLoader::loadGltfFile("C:\\Users\\Chen\\Documents\\Code\\HiveGL\\assets\\Models\\TriMesh\\dragon.gltf")
	};
	for (const auto& pNode : Nodes) setMaterial(pNode, g_ObjMat);
	g_SizeofNodes = Nodes.size();

	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, (float)App.getWidth() / (float)App.getHeight());
	const auto& pCameraController = std::make_shared<Elaina::CArcballController>();
	pCameraController->control(pCamera);
	App.addInputHandler(pCameraController);
	App.addInputHandler(std::make_shared<CMyInputHandler>());

	const auto& pDirLight = std::make_shared<Elaina::SDirectionalLight>();
	pDirLight->_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	pDirLight->_LightIntensity = 10.0f;
	pDirLight->_LightPos = glm::vec3(-5.0f, 10.0f, 0.0f);
	pDirLight->_LightDir = glm::vec3(0.0f, 0.0f, 0.0f) - pDirLight->_LightPos;

	g_Scene = std::make_shared<Elaina::CScene>();
	g_Scene->setCamera(pCamera);
	g_Scene->setDirectionalLight(pDirLight);
	g_Scene->setRootNode(pRootNode);

	setRenderPipeline(App.getWidth(), App.getHeight());

	float LastTime = 0.0f;
	while (!App.shouldClose())
	{
		App.pollEvents();
		Elaina::CImGui::beginFrame();
		float CurrTime = static_cast<float>(glfwGetTime());
		float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		//spdlog::info("FPS: {}", (int)(1.0f / DeltaTime));
		pTestNode->clearChilds();
		pTestNode->addChild(Nodes[g_IndexOfNodes]);
		pTestNode->setRotation(glm::vec3(1.0f, 1.0f, 1.0f) * CurrTime * 5.0f);
		g_RenderPipeline->render(g_Scene);
		renderUI();
		Elaina::CImGui::endFrame();
		App.swapBuffers();
	}
	g_Scene.reset();
	g_RenderPipeline.reset();
	g_DirShadowMapFB.reset();
	return 0;
}