#pragma once

#include <string>

struct GLFWwindow;
namespace Elaina
{
	class CElainaApp
	{
	public:
		CElainaApp(int vWidth, int vHeight, const std::string& vAppName = "Elaina");
		
		void run();
	
	private:
		GLFWwindow* m_pWindow;
	};
}