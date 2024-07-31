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
#include "core/Node.h"
#include "core/Scene.h"
#include "core/RenderPipeline.h"
#include "core/RenderPass.h"
#include "renderpass/ForwardPbrPass.h"
#include "renderpass/PostProcessPass.h"
#include "light/Light.h"
#include "controller/ArcballController.h"
#include "primitive/Primitive.h"
#include "loader/ModelLoader.h"
#include "safe.h"

size_t g_IndexOfNodes = 0;
size_t g_SizeofNodes = 0;

class CMyInputHandler : public Elaina::CInputHandler
{
public:
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

std::shared_ptr<Elaina::CShaderProgram> createShaderProgram(const std::string& vVertPath, const std::string& vFragPath)
{
	const auto& pProgram = std::make_shared<Elaina::CShaderProgram>();
	pProgram->attachShader(Elaina::CShaderProgram::EShaderType::VERTEX, vVertPath);
	pProgram->attachShader(Elaina::CShaderProgram::EShaderType::FRAGMENT, vFragPath);
	pProgram->linkProgram();
	return pProgram;
}

void setShaderProgram(const std::shared_ptr<Elaina::CNode>& vRootNode, const std::shared_ptr<Elaina::CShaderProgram>& vShaderProgram)
{
	Elaina::CNode::traverse(vRootNode, [&](const std::shared_ptr<Elaina::CNode>& vNode) {
		for (const auto& pMesh : vNode->getMeshes())
			pMesh->setShaderProgram(vShaderProgram);
	});
}

int main()
{
	Elaina::CGlfwWindow App;
	_ASSERTE(App.init(800, 600));
	Elaina::CFrameBuffer::initDefaultFrameBuffer(App.getWidth(), App.getHeight(), 0);

	const auto& pProgram = createShaderProgram("shaders\\pbr.vert", "shaders\\pbr.frag");
	pProgram->setUniform("uAlbedo", glm::vec3(1.0f, 1.0f, 0.0f));
	pProgram->setUniform("uMetallic", 0.0f);
	pProgram->setUniform("uRoughness", 1.0f);
	pProgram->setUniform("uAo", 1.0f);
	//pProgram->setUniform("uAo1", 1.0f);

	std::vector<std::shared_ptr<Elaina::CNode>> Nodes{
		//createNode(Elaina::CPrimitive::createQuad()),
		createNode(Elaina::CPrimitive::createTorus()),
		createNode(Elaina::CPrimitive::createCube()),
		createNode(Elaina::CPrimitive::createSphere()),
		Elaina::CModelLoader::loadGltfFile("C:\\Users\\Chen\\Documents\\Code\\HiveGL\\assets\\Models\\TriMesh\\dragon.gltf")
	};
	for (const auto& pNode : Nodes) setShaderProgram(pNode, pProgram);
	g_SizeofNodes = Nodes.size();

	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, (float)App.getWidth() / (float)App.getHeight());
	const auto& pCameraController = std::make_shared<Elaina::CArcballController>();
	pCameraController->control(pCamera);
	App.addInputHandler(pCameraController);
	App.addInputHandler(std::make_shared<CMyInputHandler>());

	const auto& pDirLight = std::make_shared<Elaina::SDirectionalLight>();
	pDirLight->_LightColor = glm::vec3(10.0f, 10.0f, 10.0f);
	pDirLight->_LightPos = glm::vec3(0.0f, 0.0f, 3.0f);

	const auto& pScene = std::make_shared<Elaina::CScene>();
	pScene->setCamera(pCamera);
	pScene->setDirectionalLight(pDirLight);

	const auto& pRenderPipeline = std::make_shared<Elaina::CRenderPipeline>();
	pRenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::createFrameBuffer(App.getWidth(), App.getHeight(), 1, true, false));
	pRenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::createFrameBuffer(App.getWidth(), App.getHeight(), 1, true, false));
	pRenderPipeline->addFrameBuffer(Elaina::CFrameBuffer::getDefaultFrameBuffer());
	pRenderPipeline->addRenderPass(std::make_shared<Elaina::CForwardPbrPass>(), 0);
	pRenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(createShaderProgram(
		"shaders\\postProcess.vert",
		"shaders\\postProcessInversion.frag"
	)), 1);
	pRenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(createShaderProgram(
		"shaders\\postProcess.vert",
		"shaders\\postProcessInversion.frag"
	)), 0);
	pRenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(createShaderProgram(
		"shaders\\postProcess.vert",
		"shaders\\postProcessInversion.frag"
	)), 1);
	pRenderPipeline->addRenderPass(std::make_shared<Elaina::CPostProcessPass>(createShaderProgram(
		"shaders\\postProcess.vert",
		"shaders\\postProcessInversion.frag"
	)), 2);
	float LastTime = 0.0f;
	while (!App.shouldClose())
	{
		App.pollEvents();
		float CurrTime = static_cast<float>(glfwGetTime());
		float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		//spdlog::info("FPS: {}", (int)(1.0f / DeltaTime));
		pScene->setRootNode(Nodes[g_IndexOfNodes]);
		pRenderPipeline->render(pScene);
		App.swapBuffers();
	}
	return 0;
}