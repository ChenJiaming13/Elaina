#include "pch.h"
#include "WindowSystem.h"

using namespace Elaina::core;

bool CWindowSystem::init(const SWindowSystemCreateInfo& vInfo)
{
	if (m_pWindow != nullptr)
	{
		spdlog::warn("window has been initialized");
		return false;
	}
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vInfo._GLMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vInfo._GLMinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_pWindow = glfwCreateWindow(vInfo._Width, vInfo._Height, vInfo._Title, nullptr, nullptr);
	if (m_pWindow == nullptr)
	{
		spdlog::error("failed to create GLFW window");
		cleanup();
		return false;
	}
	int Width = 0, Height = 0;
	glfwGetFramebufferSize(m_pWindow, &Width, &Height);
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwMakeContextCurrent(m_pWindow);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		spdlog::error("failed to initialize GLAD");
		cleanup();
		return false;
	}
	if (vInfo._EnableDebug) __setupDebug();
	return true;
}

void CWindowSystem::cleanup()
{
	if (m_pWindow != nullptr)
	{
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
	glfwTerminate();
}

bool CWindowSystem::shouldClose() const
{
	if (m_pWindow == nullptr) return true;
	return glfwWindowShouldClose(m_pWindow);
}

void CWindowSystem::pollEvents()
{
	glfwPollEvents();
}

void CWindowSystem::swapBuffers() const
{
	if (m_pWindow == nullptr) return;
	glfwSwapBuffers(m_pWindow);
}

std::pair<int, int> CWindowSystem::getExtent() const
{
	int Width, Height;
	glfwGetFramebufferSize(m_pWindow, &Width, &Height);
	return { Width, Height };
}

float CWindowSystem::getAspect() const
{
	auto [Width, Height] = getExtent();
	return static_cast<float>(Width) / static_cast<float>(Height);
}

void messageCallback(GLenum vSource, GLenum vType, GLuint vErrorCode, GLenum vSeverity, GLsizei vLength, GLchar const* vMessage, void const* vUserParam)
{
	auto const SourceStr = [vSource]() {
		switch (vSource)
		{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			default: return "";
		}
	}();

	auto const TypeStr = [vType]() {
		switch (vType)
		{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			default: return "";
		}
	}();

	auto const SeverityStr = [vSeverity]() {
		switch (vSeverity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "";
		}
	}();

	const std::string DebugInfo = std::format("<{}>[{}]({}:{}):{}", SeverityStr, SourceStr, TypeStr, vErrorCode, vMessage);
	if (vSeverity == GL_DEBUG_SEVERITY_LOW) spdlog::warn(DebugInfo);
	else if (vSeverity == GL_DEBUG_SEVERITY_MEDIUM || vSeverity == GL_DEBUG_SEVERITY_HIGH) spdlog::error(DebugInfo);
	else spdlog::info(DebugInfo);
}

void CWindowSystem::__setupDebug()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(messageCallback, nullptr);
}
