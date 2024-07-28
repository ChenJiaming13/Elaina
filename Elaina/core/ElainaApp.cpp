#include "pch.h"
#include "ElainaApp.h"

Elaina::CElainaApp::CElainaApp(int vWidth, int vHeight, const std::string& vAppName) :m_pWindow(nullptr)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_pWindow = glfwCreateWindow(vWidth, vHeight, vAppName.c_str(), nullptr, nullptr);
	if (m_pWindow == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("failed to create GLFW window");
	}
	glfwMakeContextCurrent(m_pWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
		glfwTerminate();
		throw std::runtime_error("failed to initialize GLAD");
	}
}

void Elaina::CElainaApp::run()
{
	float LastTime = 0.0f;
	while (!glfwWindowShouldClose(m_pWindow))
	{
		float CurrTime = static_cast<float>(glfwGetTime());
		float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		spdlog::info("FPS: {}", (int)(1.0f / DeltaTime));
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
}
