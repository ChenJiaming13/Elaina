#pragma once

namespace Elaina
{
	class CImGui
	{
	public:
		static void init(GLFWwindow* vWindow);
		static void beginFrame();
		static void endFrame();
		static void shutdown();
		static bool wantCaptureMouse();
		static bool wantCaptureKeyboard();
	};
}