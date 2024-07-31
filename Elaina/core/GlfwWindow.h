#pragma once

#include <string>

struct GLFWwindow;
namespace Elaina
{
	class CInputHandler;
	class CGlfwWindow
	{
	public:
		CGlfwWindow();
		~CGlfwWindow();

		bool init(int vWidth, int vHeight, const std::string & vAppName = "Elaina");
		bool shouldClose() const;
		void pollEvents() const;
		void swapBuffers() const;
		void addInputHandler(const std::shared_ptr<CInputHandler>& vInputHandler);
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }

	private:
		void __registerCallbacks() const;
		void __cleanup();

		static void __windowSizeChangeCallback(GLFWwindow* vWindow, int vWidth, int vHeight);
		static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
		static void __mouseScrollCallback(GLFWwindow* vWindow, double vXoffset, double vYoffset);
		static void __mouseMoveCallback(GLFWwindow* vWindow, double vXpos, double vYpos);
		static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMods);

		GLFWwindow* m_pWindow;
		int m_Width;
		int m_Height;
		std::vector<std::shared_ptr<CInputHandler>> m_InputHandlers;
	};
}
