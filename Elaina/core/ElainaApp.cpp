#include "pch.h"
#include "ElainaApp.h"

Elaina::CElainaApp::CElainaApp() :m_pWindow(nullptr)
{
}

Elaina::CElainaApp::~CElainaApp()
{
	__cleanup();
}

bool Elaina::CElainaApp::init(int vWidth, int vHeight, const std::string& vAppName)
{
	if (m_pWindow != nullptr)
	{
		spdlog::warn("window has been initialized");
		return false;
	}
	if (vWidth <= 0 || vHeight <= 0)
	{
		spdlog::error("window size must be greater than 0");
		return false;
	}
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_pWindow = glfwCreateWindow(vWidth, vHeight, vAppName.c_str(), nullptr, nullptr);
	if (m_pWindow == nullptr)
	{
		glfwTerminate();
		spdlog::error("failed to create GLFW window");
		return false;
	}
	glfwMakeContextCurrent(m_pWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		__cleanup();
		spdlog::error("failed to initialize GLAD");
		return false;
	}
	return true;
}

bool Elaina::CElainaApp::shouldClose() const
{
	_ASSERTE(m_pWindow != nullptr);
	return glfwWindowShouldClose(m_pWindow);
}

void Elaina::CElainaApp::pollEvents() const
{
	glfwPollEvents();
}

void Elaina::CElainaApp::swapBuffers() const
{
	_ASSERTE(m_pWindow != nullptr);
	glfwSwapBuffers(m_pWindow);
}

void Elaina::CElainaApp::__cleanup()
{
	if (m_pWindow != nullptr)
	{
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
	glfwTerminate();
}
