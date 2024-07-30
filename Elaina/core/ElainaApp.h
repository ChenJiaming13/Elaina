#pragma once

#include <string>

struct GLFWwindow;
namespace Elaina
{
	class CInputHandler;
	class CElainaApp
	{
	public:
		CElainaApp();
		~CElainaApp();

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

		friend void windowSizeChangeCallback(GLFWwindow* vWindow, int vWidth, int vHeight);
		friend void mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
		friend void mouseScrollCallback(GLFWwindow* vWindow, double vXoffset, double vYoffset);
		friend void mouseMoveCallback(GLFWwindow* vWindow, double vXpos, double vYpos);
		friend void keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMods);

		GLFWwindow* m_pWindow;
		int m_Width;
		int m_Height;
		std::vector<std::shared_ptr<CInputHandler>> m_InputHandlers;
	};
}
