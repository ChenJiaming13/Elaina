#include "pch.h"

using namespace Elaina;

TEST(Test_WindowSystem, NT_DefaultInfo)
{
	core::CWindowSystem WindowSystem;
	constexpr core::SWindowSystemCreateInfo CreateInfo;
	EXPECT_TRUE(WindowSystem.init(CreateInfo));
	auto [Width, Height] = WindowSystem.getExtent();
	EXPECT_EQ(Width, CreateInfo._Width);
	EXPECT_EQ(Height, CreateInfo._Height);
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
	constexpr core::SWindowSystemCreateInfo CreateInfo
	{
		._Width = 1280,
		._Height = 780,
		._Title = "Custom Title"
	};
	EXPECT_TRUE(WindowSystem.init(CreateInfo));
	auto [Width, Height] = WindowSystem.getExtent();
	EXPECT_EQ(Width, CreateInfo._Width);
	EXPECT_EQ(Height, CreateInfo._Height);
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
	constexpr core::SWindowSystemCreateInfo CreateInfo;
	EXPECT_TRUE(WindowSystem.init(CreateInfo));
	EXPECT_FALSE(WindowSystem.init(CreateInfo));
	EXPECT_FALSE(WindowSystem.init(CreateInfo));
	WindowSystem.cleanup();
}

TEST(Test_WindowSystem, DT_InvalidWindowSize)
{
	core::CWindowSystem WindowSystem;
	constexpr core::SWindowSystemCreateInfo CreateInfo
	{
		._Width = -1,
		._Height = 780
	};
	EXPECT_FALSE(WindowSystem.init(CreateInfo));
	auto [Width, Height] = WindowSystem.getExtent();
	EXPECT_EQ(Width, 0);
	EXPECT_EQ(Height, 0);
	EXPECT_TRUE(WindowSystem.shouldClose());
	WindowSystem.cleanup();
}

TEST(Test_WindowSystem, DT_NoInit)
{
	core::CWindowSystem WindowSystem;
	WindowSystem.cleanup();
}