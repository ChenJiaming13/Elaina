#pragma once

#include <map>
#include <set>
#include <glad/glad.h>

namespace Elaina::gl
{
	using TextureHandle = GLuint;

	constexpr auto INVALID_TEXTURE_HANDLE = std::numeric_limits<TextureHandle>::max();

	struct SExtent2D
	{
		GLsizei _Width = 0;
		GLsizei _Height = 0;
	};

	struct STexture2DCreateInfo
	{
		static constexpr auto AutoCalcMipLevels = 0;

		SExtent2D _Extent{};
		GLsizei _MipLevels = 1; // auto calc mip levels if set AutoCalcMipLevels
		GLenum _InternalFormat = GL_RGB8;
		GLint _WrapS = GL_REPEAT;
		GLint _WrapT = GL_REPEAT;
		GLint _FilterMin = GL_LINEAR;
		GLint _FilterMag = GL_LINEAR;
	};

	struct STexture2DSubArea
	{
		GLint _MipLevel = 0;
		GLint _OffsetX = 0;
		GLint _OffsetY = 0;
		SExtent2D _Extent;
		GLenum _Format = GL_RGB;
		GLenum _DataType = GL_UNSIGNED_BYTE;
	};

	class CTextureManager
	{
	public:
		~CTextureManager() { cleanup(); }

		TextureHandle getOrCreateTexture(const std::string& vImagePath, bool vFlipY = false);

		TextureHandle createTexture(const STexture2DCreateInfo& vCreateInfo);

		void fillTextureData(TextureHandle vHandle, const STexture2DSubArea& vArea, const void* vPixels) const;

		void bindSampler2D(TextureHandle vHandle, GLuint vBindUnit) const;

		void bindImage2D(TextureHandle vHandle, GLuint vBindUnit, GLenum vAccess, GLint vLevel) const;

		void deleteTexture(TextureHandle vHandle);

		[[nodiscard]] SExtent2D queryTextureExtent(TextureHandle vHandle, GLint vLevel = 0) const;

		[[nodiscard]] GLint queryTextureNumLevels(TextureHandle vHandle) const;

		void cleanup();

	private:
		std::set<TextureHandle> m_Handles;
		std::map<std::string, TextureHandle> m_PathToHandle;
		std::map<TextureHandle, std::string> m_HandleToPath;
	};
}