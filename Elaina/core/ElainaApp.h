#pragma once

#include <string>

struct GLFWwindow;
namespace Elaina
{
	class CElainaApp
	{
	public:
		CElainaApp();
		~CElainaApp();

		bool init(int vWidth, int vHeight, const std::string & vAppName = "Elaina");
		bool shouldClose() const;
		void pollEvents() const;
		void swapBuffers() const;
	
	private:
		void __cleanup();

		GLFWwindow* m_pWindow;
	};
}