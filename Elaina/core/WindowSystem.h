#pragma once

struct GLFWwindow;

namespace Elaina::core
{
	struct SWindowSystemCreateInfo
	{
		int _Width = 800;
		int _Height = 600;
		const char* _Title = "Elaina";
		int _GLMajorVersion = 4;
		int _GLMinorVersion = 6;
		bool _EnableDebug = true;
	};

	class CWindowSystem
	{
	public:
		~CWindowSystem() { cleanup(); }

		bool init(const SWindowSystemCreateInfo& vInfo={});
		void cleanup();
		[[nodiscard]] bool shouldClose() const;
		static void pollEvents();
		void swapBuffers() const;
		[[nodiscard]] std::pair<int, int> getExtent() const;
		[[nodiscard]] float getAspect() const;

	private:
		static void __setupDebug();

		GLFWwindow* m_pWindow = nullptr;
	};
}