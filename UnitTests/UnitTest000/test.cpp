#include "pch.h"
#include "pch.h"

using namespace Elaina;

TEST(Test_WindowSystem, NT_DefaultInfo)
{
	core::CWindowSystem WindowSystem;
	constexpr core::SWindowSystemInfo Info;
	EXPECT_TRUE(WindowSystem.init(Info));
	EXPECT_EQ(WindowSystem.getWidth(), Info._Width);
	EXPECT_EQ(WindowSystem.getHeight(), Info._Height);
	while (!WindowSystem.shouldClose())
	{
		core::CWindowSystem::pollEvents();
		WindowSystem.swapBuffers();
	}
	WindowSystem.cleanup();
}

TEST(Test_WindowSystem, NT_CustomInfo)
{
	core::CWindowSystem WindowSystem;
	constexpr core::SWindowSystemInfo Info
	{
		._Width = 1280,
		._Height = 780,
		._Title = "Custom Title"
	};
	EXPECT_TRUE(WindowSystem.init(Info));
	EXPECT_EQ(WindowSystem.getWidth(), Info._Width);
	EXPECT_EQ(WindowSystem.getHeight(), Info._Height);
	while (!WindowSystem.shouldClose())
	{
		core::CWindowSystem::pollEvents();
		WindowSystem.swapBuffers();
	}
	WindowSystem.cleanup();
}

TEST(Test_WindowSystem, DT_InitTwice)
{
	core::CWindowSystem WindowSystem;
	constexpr core::SWindowSystemInfo Info;
	EXPECT_TRUE(WindowSystem.init(Info));
	EXPECT_FALSE(WindowSystem.init(Info));
	EXPECT_FALSE(WindowSystem.init(Info));
	WindowSystem.cleanup();
}

TEST(Test_WindowSystem, DT_InvalidWindowSize)
{
	core::CWindowSystem WindowSystem;
	constexpr core::SWindowSystemInfo Info
	{
		._Width = -1,
		._Height = 780
	};
	EXPECT_FALSE(WindowSystem.init(Info));
	EXPECT_EQ(WindowSystem.getWidth(), 0);
	EXPECT_EQ(WindowSystem.getHeight(), 0);
	EXPECT_TRUE(WindowSystem.shouldClose());
	WindowSystem.cleanup();
}

TEST(Test_WindowSystem, DT_NoInit)
{
	core::CWindowSystem WindowSystem;
	WindowSystem.cleanup();
}