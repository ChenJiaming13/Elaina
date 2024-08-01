#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "base/Framebuffer.h"
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
#include "light/Light.h"
#include "controller/ArcballController.h"
#include "primitive/Primitive.h"
#include "loader/ModelLoader.h"
#include "safe.h"

size_t g_IndexOfNodes = 0;
size_t g_SizeofNodes = 0;
std::shared_ptr<Elaina::CScene> g_Scene = nullptr;
std::shared_ptr<Elaina::CRenderPipeline> g_RenderPipeline = nullptr;

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

	g_RenderPipeline = std::make_shared<Elaina::CRenderPipeline>();
	g_RenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::createFrameBuffer(1024, 1024, 0, true, false));
	g_RenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::createFrameBuffer(vWidth, vHeight, 4, true, false));
	g_RenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::getDefaultFrameBuffer());
	g_RenderPipeline->addRenderPass(pDirShadowMapPass, 0, false);
	g_RenderPipeline->addRenderPass(pDeferredGeoPass, 1);
	g_RenderPipeline->addRenderPass(pDeferredLitPass, 2);
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

int main()
{
	Elaina::CGlfwWindow App;
	_ASSERTE(App.init(800, 600));
	Elaina::CFrameBuffer::initDefaultFrameBuffer(App.getWidth(), App.getHeight(), 0);

	const auto& pPlaneMat = std::make_shared<Elaina::SPbrMaterial>();
	pPlaneMat->_Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	pPlaneMat->_Metallic = 0.5f;
	pPlaneMat->_Roughness = 0.1f;
	pPlaneMat->_Ao = 0.1f;

	const auto& pObjMat = std::make_shared<Elaina::SPbrMaterial>();
	pObjMat->_Albedo = glm::vec3(1.0f, 0.0f, 0.0f);
	pObjMat->_Metallic = 0.5f;
	pObjMat->_Roughness = 1.0f;
	pObjMat->_Ao = 0.1f;

	const auto& pPlaneNode = createNode(Elaina::CPrimitive::createPlane());
	pPlaneNode->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
	pPlaneNode->setPosition(glm::vec3(0.0f, -5.0f, 0.0f));
	setMaterial(pPlaneNode, pPlaneMat);
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
	for (const auto& pNode : Nodes) setMaterial(pNode, pObjMat);
	g_SizeofNodes = Nodes.size();

	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, (float)App.getWidth() / (float)App.getHeight());
	const auto& pCameraController = std::make_shared<Elaina::CArcballController>();
	pCameraController->control(pCamera);
	App.addInputHandler(pCameraController);
	App.addInputHandler(std::make_shared<CMyInputHandler>());

	const auto& pDirLight = std::make_shared<Elaina::SDirectionalLight>();
	pDirLight->_LightColor = glm::vec3(10.0f, 10.0f, 10.0f);
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
		float CurrTime = static_cast<float>(glfwGetTime());
		float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		//spdlog::info("FPS: {}", (int)(1.0f / DeltaTime));
		pTestNode->clearChilds();
		pTestNode->addChild(Nodes[g_IndexOfNodes]);
		pTestNode->setRotation(glm::vec3(1.0f, 1.0f, 1.0f) * CurrTime * 5.0f);
		g_RenderPipeline->render(g_Scene);
		App.swapBuffers();
	}
	g_Scene.reset();
	g_RenderPipeline.reset();
	return 0;
}