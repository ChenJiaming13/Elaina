#pragma once

struct GLFWwindow;

namespace Elaina::core
{
	struct SWindowSystemInfo
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

		bool init(const SWindowSystemInfo& vInfo={});
		void cleanup();
		[[nodiscard]] bool shouldClose() const;
		static void pollEvents();
		void swapBuffers() const;
		[[nodiscard]] int getWidth() const { return m_Info._Width; }
		[[nodiscard]] int getHeight() const { return m_Info._Height; }

	private:
		static void __setupDebug();

		SWindowSystemInfo m_Info{};
		GLFWwindow* m_pWindow = nullptr;
	};
}