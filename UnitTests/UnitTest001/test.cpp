#include "pch.h"

using namespace Elaina;

TEST(Test_TextureManager, NT_CreateTextureFromImageFile)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CTextureManager Manager;
	const std::string ImagePath = "texture.png";
	const auto Handle = Manager.getOrCreateTexture(ImagePath);
	ASSERT_NE(Handle, Elaina::gl::INVALID_TEXTURE_HANDLE);
}

TEST(Test_TextureManager, NT_CachedImageTexture)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CTextureManager Manager;
	const std::string ImagePath = "texture.png";
	const auto Handle1 = Manager.getOrCreateTexture(ImagePath);
	ASSERT_NE(Handle1, Elaina::gl::INVALID_TEXTURE_HANDLE);
	const auto Handle2 = Manager.getOrCreateTexture(ImagePath);
	ASSERT_EQ(Handle1, Handle2);
}

TEST(Test_TextureManager, NT_CreateEmptyRGBATexture)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CTextureManager Manager;
	constexpr gl::STexture2DCreateInfo TextureInfo{
		._Extent = { 100, 200 },
		._InternalFormat = GL_RGBA8,
	};
	const auto Handle = Manager.createTexture(TextureInfo);
	ASSERT_NE(Handle, Elaina::gl::INVALID_TEXTURE_HANDLE);
}

TEST(Test_TextureManager, NT_CreateEmptyGrayTexture)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CTextureManager Manager;
	constexpr gl::STexture2DCreateInfo TextureInfo{
		._Extent = { 100, 200 },
		._InternalFormat = GL_R8,
	};
	const auto Handle = Manager.createTexture(TextureInfo);
	ASSERT_NE(Handle, Elaina::gl::INVALID_TEXTURE_HANDLE);
}

TEST(Test_TextureManager, DT_InvalidTextureSize)
{
	core::CWindowSystem WindowSystem;
	ASSERT_TRUE(WindowSystem.init());

	gl::CTextureManager Manager;
	constexpr gl::STexture2DCreateInfo TextureInfo{
		._Extent = { 0, 200 },
	};
	const auto Handle = Manager.createTexture(TextureInfo);
	ASSERT_NE(Handle, Elaina::gl::INVALID_TEXTURE_HANDLE);
	auto [Width, Height] = Manager.queryTextureExtent(Handle);
	ASSERT_EQ(Width, 0);
	ASSERT_EQ(Height, 0);
}