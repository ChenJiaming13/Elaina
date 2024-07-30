#include "pch.h"
#include "ElainaApp.h"
#include "controller/CameraController.h"

void Elaina::windowSizeChangeCallback(GLFWwindow* vWindow, int vWidth, int vHeight)
{
	CElainaApp* pApp = (CElainaApp*)glfwGetWindowUserPointer(vWindow);
	pApp->m_Width = vWidth;
	pApp->m_Height = vHeight;
	if (pApp->m_pCamController != nullptr)
	{
		pApp->m_pCamController->onWindowSizeChange(vWidth, vHeight);
	}
}

void Elaina::mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods)
{
	CElainaApp* pApp = (CElainaApp*)glfwGetWindowUserPointer(vWindow);
	if (pApp->m_pCamController != nullptr)
	{
		if (vAction == GLFW_PRESS) pApp->m_pCamController->onMouseButtonDown(vButton);
		else if (vAction == GLFW_RELEASE) pApp->m_pCamController->onMouseButtonUp(vButton);
	}
}

void Elaina::mouseScrollCallback(GLFWwindow* vWindow, double vXoffset, double vYoffset)
{
	CElainaApp* pApp = (CElainaApp*)glfwGetWindowUserPointer(vWindow);
	if (pApp->m_pCamController != nullptr)
	{
		pApp->m_pCamController->onMouseScroll((float)vXoffset, (float)vYoffset);
	}
}

void Elaina::mouseMoveCallback(GLFWwindow* vWindow, double vXpos, double vYpos)
{
	CElainaApp* pApp = (CElainaApp*)glfwGetWindowUserPointer(vWindow);
	if (pApp->m_pCamController != nullptr)
	{
		pApp->m_pCamController->onMouseMove((float)vXpos, (float)vYpos);
	}
}

void Elaina::keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMods)
{
	CElainaApp* pApp = (CElainaApp*)glfwGetWindowUserPointer(vWindow);
	if (pApp->m_pCamController != nullptr)
	{
		if (vAction == GLFW_PRESS) pApp->m_pCamController->onKeyDown(vKey);
		else if (vAction == GLFW_RELEASE) pApp->m_pCamController->onKeyUp(vKey);
	}
}

Elaina::CElainaApp::CElainaApp() :m_pWindow(nullptr), m_pCamController(nullptr), m_Width(0), m_Height(0)
{
}

Elaina::CElainaApp::~CElainaApp()
{
	m_pCamController.reset();
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
	glfwGetFramebufferSize(m_pWindow, &m_Width, &m_Height);
	glfwSetWindowUserPointer(m_pWindow, this);
	__registerCallbacks();
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

void Elaina::CElainaApp::__registerCallbacks() const
{
	glfwSetFramebufferSizeCallback(m_pWindow, windowSizeChangeCallback);
	glfwSetMouseButtonCallback(m_pWindow, mouseButtonCallback);
	glfwSetScrollCallback(m_pWindow, mouseScrollCallback);
	glfwSetCursorPosCallback(m_pWindow, mouseMoveCallback);
	glfwSetKeyCallback(m_pWindow, keyCallback);
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
