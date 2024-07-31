#include "pch.h"
#include "GlfwWindow.h"
#include "InputHandler.h"

void Elaina::windowSizeChangeCallback(GLFWwindow* vWindow, int vWidth, int vHeight)
{
	CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
	pApp->m_Width = vWidth;
	pApp->m_Height = vHeight;
	for (const auto& pInputHandler : pApp->m_InputHandlers)
	{
		if (pInputHandler == nullptr) continue;
		pInputHandler->onWindowSizeChange(vWidth, vHeight);
	}
}

void Elaina::mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods)
{
	CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
	for (const auto& pInputHandler : pApp->m_InputHandlers)
	{
		if (pInputHandler == nullptr) continue;
		if (vAction == GLFW_PRESS) pInputHandler->onMouseButtonDown(vButton);
		else if (vAction == GLFW_RELEASE) pInputHandler->onMouseButtonUp(vButton);
	}
}

void Elaina::mouseScrollCallback(GLFWwindow* vWindow, double vXoffset, double vYoffset)
{
	CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
	for (const auto& pInputHandler : pApp->m_InputHandlers)
	{
		if (pInputHandler == nullptr) continue;
		pInputHandler->onMouseScroll((float)vXoffset, (float)vYoffset);
	}
}

void Elaina::mouseMoveCallback(GLFWwindow* vWindow, double vXpos, double vYpos)
{
	CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
	for (const auto& pInputHandler : pApp->m_InputHandlers)
	{
		if (pInputHandler == nullptr) continue;
		pInputHandler->onMouseMove((float)vXpos, (float)vYpos);
	}
}

void Elaina::keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMods)
{
	CGlfwWindow* pApp = (CGlfwWindow*)glfwGetWindowUserPointer(vWindow);
	for (const auto& pInputHandler : pApp->m_InputHandlers)
	{
		if (pInputHandler == nullptr) continue;
		if (vAction == GLFW_PRESS) pInputHandler->onKeyDown(vKey);
		else if (vAction == GLFW_RELEASE) pInputHandler->onKeyUp(vKey);
	}
}

Elaina::CGlfwWindow::CGlfwWindow() :m_pWindow(nullptr), m_Width(0), m_Height(0), m_InputHandlers{}
{
}

Elaina::CGlfwWindow::~CGlfwWindow()
{
	m_InputHandlers.clear();
	__cleanup();
}

bool Elaina::CGlfwWindow::init(int vWidth, int vHeight, const std::string& vAppName)
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

bool Elaina::CGlfwWindow::shouldClose() const
{
	_ASSERTE(m_pWindow != nullptr);
	return glfwWindowShouldClose(m_pWindow);
}

void Elaina::CGlfwWindow::pollEvents() const
{
	glfwPollEvents();
}

void Elaina::CGlfwWindow::swapBuffers() const
{
	_ASSERTE(m_pWindow != nullptr);
	glfwSwapBuffers(m_pWindow);
}

void Elaina::CGlfwWindow::addInputHandler(const std::shared_ptr<CInputHandler>& vInputHandler)
{
	if (vInputHandler == nullptr) return;
	m_InputHandlers.push_back(vInputHandler);
}

void Elaina::CGlfwWindow::__registerCallbacks() const
{
	glfwSetFramebufferSizeCallback(m_pWindow, windowSizeChangeCallback);
	glfwSetMouseButtonCallback(m_pWindow, mouseButtonCallback);
	glfwSetScrollCallback(m_pWindow, mouseScrollCallback);
	glfwSetCursorPosCallback(m_pWindow, mouseMoveCallback);
	glfwSetKeyCallback(m_pWindow, keyCallback);
}

void Elaina::CGlfwWindow::__cleanup()
{
	if (m_pWindow != nullptr)
	{
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
	glfwTerminate();
}
