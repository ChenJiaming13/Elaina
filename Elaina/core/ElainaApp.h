#pragma once

#include <string>

struct GLFWwindow;
namespace Elaina
{
	class CCameraController;
	class CElainaApp
	{
	public:
		CElainaApp();
		~CElainaApp();

		bool init(int vWidth, int vHeight, const std::string & vAppName = "Elaina");
		bool shouldClose() const;
		void pollEvents() const;
		void swapBuffers() const;
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		void setCameraController(const std::shared_ptr<CCameraController>& vCameraController) { m_pCamController = vCameraController; }

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
		std::shared_ptr<CCameraController> m_pCamController;
	};
}
