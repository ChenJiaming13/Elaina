#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "base/VertexArrayObject.h"
#include "core/GlfwWindow.h"
#include "core/Camera.h"
#include "controller/ArcballController.h"
#include "controller/FPSController.h"
#include "ui/ImGui.h"

std::shared_ptr<Elaina::CFrameBuffer> g_FrameBuffer = nullptr;

bool readBinFile(const std::string& vFileName, std::vector<char>& voBuffer)
{
	std::ifstream File(vFileName, std::ios::binary);
	if (!File)
	{
		std::cerr << "cannot open this file: " << vFileName << '\n';
		return false;
	}
	File.seekg(0, std::ios::end);
	const std::streamsize Size = File.tellg();
	File.seekg(0, std::ios::beg);
	voBuffer.resize(Size);
	if (!File.read(voBuffer.data(), Size))
	{
		std::cerr << "fail to read file:" << vFileName << '\n';
	}
	File.close();
	return true;
}

bool readHeightMap(const std::string& vFileName, size_t& voSize, std::vector<float>& voData)
{
	std::vector<char> Buffer;
	readBinFile(vFileName, Buffer);
	if (Buffer.size() % sizeof(float) != 0)
	{
		std::cerr << "cannot parse the height map file: " << vFileName << '\n';
		return false;
	}
	const size_t Count = Buffer.size() / sizeof(float);
	voSize = static_cast<size_t>(std::sqrt(Count));
	if (voSize * voSize != Count)
	{
		std::cerr << "the height map file is invalid: " << vFileName << '\n';
	}
	voData.resize(Count);
	std::memcpy(voData.data(), Buffer.data(), Buffer.size());
	return true;
}

std::shared_ptr<Elaina::CVertexArrayObject> generateMesh(size_t vSize, const std::vector<float>& vData)
{
	std::vector<float> Vertices;
	for (size_t z = 0; z < vSize; ++z)
	{
		for (size_t x = 0; x < vSize; ++x)
		{
			float Height = vData[z * vSize + x];
			Vertices.push_back(static_cast<float>(x));
			Vertices.push_back(Height);
			Vertices.push_back(static_cast<float>(z));
		}
	}

	std::vector<unsigned int> Indices;
	for (size_t z = 0; z < vSize - 1; ++z)
	{
		for (size_t x = 0; x < vSize - 1; ++x)
		{
			const unsigned int TopLeft = static_cast<unsigned int>(z * vSize + x);
			const unsigned int TopRight = TopLeft + 1;
			const unsigned int BottomLeft = static_cast<unsigned int>((z + 1) * vSize + x);
			const unsigned int BottomRight = BottomLeft + 1;

			Indices.push_back(TopLeft);
			Indices.push_back(BottomLeft);
			Indices.push_back(TopRight);

			Indices.push_back(TopRight);
			Indices.push_back(BottomLeft);
			Indices.push_back(BottomRight);
		}
	}

	return Elaina::CVertexArrayObject::createVAO(Vertices, Indices, std::vector<int>{3}, GL_TRIANGLES, GL_STATIC_DRAW);
}

class CMyHandler final : public Elaina::CInputHandler
{
public:
	void onWindowSizeChangeV(int vWidth, int vHeight) override
	{
		if (g_FrameBuffer != nullptr)
		{
			g_FrameBuffer->resize(vWidth, vHeight);
		}
	}
};

int main()
{
	Elaina::CGlfwWindow App;
	_ASSERTE(App.init(800, 600, "Terrain Rendering"));
	Elaina::CFrameBuffer::initDefaultFrameBuffer(App.getWidth(), App.getHeight(), 0);
	g_FrameBuffer = Elaina::CFrameBuffer::getDefaultFrameBuffer();
	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, static_cast<float>(App.getWidth()) / static_cast<float>(App.getHeight()));
	pCamera->setFar(100000.0f);
	//const auto& pCameraController = std::make_shared<Elaina::CArcballController>();
	const auto& pCameraController = std::make_shared<Elaina::CFPSController>();
	pCameraController->controlV(pCamera);
	App.addInputHandler(pCameraController);
	App.addInputHandler(std::make_shared<CMyHandler>());

	const auto pShaderProgram = Elaina::CShaderProgram::createShaderProgram(
		"shaders/simple.vs.glsl",
		"shaders/simple.fs.glsl"
	);

	size_t HeightMapSize;
	std::vector<float> HeightMapData;
	_ASSERTE(readHeightMap("heightmap.save", HeightMapSize, HeightMapData));
	const auto pVAO = generateMesh(HeightMapSize, HeightMapData);

	g_FrameBuffer->bind();
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glClearDepth(1.0f));
	GL_SAFE_CALL(glClearColor(0.1f, 0.2f, 0.3f, 1.0f));
	GL_SAFE_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	float LastTime = 0.0f;
	while (!App.shouldClose())
	{
		const float CurrTime = static_cast<float>(glfwGetTime());
		const float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		App.pollEvents();
		pCameraController->updateV(DeltaTime);
		Elaina::CImGui::beginFrame();
		GL_SAFE_CALL(glViewport(0, 0, g_FrameBuffer->getWidth(), g_FrameBuffer->getHeight()));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		pShaderProgram->use();
		pShaderProgram->setUniform("uModel", glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f)));
		pShaderProgram->setUniform("uViewProjection", pCamera->getProjectionMatrix() * pCamera->getViewMatrix());
		pVAO->bind();
		pVAO->draw();
		Elaina::CImGui::endFrame();
		App.swapBuffers();
	}

	return 0;
}
