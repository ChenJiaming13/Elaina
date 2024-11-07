#include "pch.h"
#include "TextureManager.h"
#include <stb_image.h>

using namespace Elaina::gl;

TextureHandle CTextureManager::getOrCreateTexture(const std::string& vImagePath, bool vFlipY)
{
	if (m_PathToHandle.contains(vImagePath)) return m_PathToHandle[vImagePath];
	int Width = 0, Height = 0, Channels = 0;
	stbi_set_flip_vertically_on_load(vFlipY);
	const auto pPixels = stbi_load(vImagePath.c_str(), &Width, &Height, &Channels, 0);
	if (pPixels == nullptr) return INVALID_TEXTURE_HANDLE;
	GLenum Format = GL_RGB, InternalFormat = GL_RGB8;
	if (Channels == 3)
	{
		Format = GL_RGB;
		InternalFormat = GL_RGB8;
	}
	else if (Channels == 4)
	{
		Format = GL_RGBA;
		InternalFormat = GL_RGBA8;
	}
	else if (Channels == 1)
	{
		Format = GL_RED;
		InternalFormat = GL_R8;
	}
	else spdlog::warn("unexpected channels: {}", Channels);
	const STexture2DCreateInfo Texture2D = {
		._Extent = { Width, Height },
		._MipLevels = STexture2DCreateInfo::AutoCalcMipLevels,
		._InternalFormat = InternalFormat,
		._WrapS = GL_REPEAT,
		._WrapT = GL_REPEAT,
		._FilterMin = GL_LINEAR,
		._FilterMag = GL_LINEAR,
	};
	const auto Handle = createTexture(Texture2D);
	if (Handle == INVALID_TEXTURE_HANDLE) return Handle;
	const STexture2DSubArea Area
	{
		._MipLevel = 0,
		._OffsetX = 0,
		._OffsetY = 0,
		._Extent = { Width, Height },
		._Format = Format,
		._DataType = GL_UNSIGNED_BYTE
	};
	fillTextureData(Handle, Area, pPixels);
	//glGenerateTextureMipmap(Handle); // TODO
	m_PathToHandle[vImagePath] = Handle;
	m_HandleToPath[Handle] = vImagePath;
	return Handle;
}

TextureHandle CTextureManager::createTexture(const STexture2DCreateInfo& vCreateInfo)
{
	TextureHandle Handle = INVALID_TEXTURE_HANDLE;
	glCreateTextures(GL_TEXTURE_2D, 1, &Handle);
	glTextureParameteri(Handle, GL_TEXTURE_WRAP_S, vCreateInfo._WrapS);
	glTextureParameteri(Handle, GL_TEXTURE_WRAP_T, vCreateInfo._WrapT);
	glTextureParameteri(Handle, GL_TEXTURE_MIN_FILTER, vCreateInfo._FilterMin);
	glTextureParameteri(Handle, GL_TEXTURE_MAG_FILTER, vCreateInfo._FilterMag);
	int MipLevels = vCreateInfo._MipLevels;
	if (MipLevels == STexture2DCreateInfo::AutoCalcMipLevels)
	{
		MipLevels = static_cast<int>(std::log2(std::max(vCreateInfo._Extent._Width, vCreateInfo._Extent._Height))) + 1;
	}
	glTextureStorage2D(Handle, MipLevels, vCreateInfo._InternalFormat, vCreateInfo._Extent._Width, vCreateInfo._Extent._Height);
	if (Handle != INVALID_TEXTURE_HANDLE) m_Handles.insert(Handle);
	return Handle;
}

void CTextureManager::fillTextureData(TextureHandle vHandle, const STexture2DSubArea& vArea, const void* vPixels) const
{
	assert(vHandle != INVALID_TEXTURE_HANDLE);
	assert(m_Handles.contains(vHandle));
	glTextureSubImage2D(
		vHandle, vArea._MipLevel,
		vArea._OffsetX, vArea._OffsetY,
		vArea._Extent._Width, vArea._Extent._Height,
		vArea._Format, vArea._DataType,
		vPixels
	);
}

void CTextureManager::bindSampler2D(TextureHandle vHandle, GLuint vBindUnit) const
{
	assert(vHandle != INVALID_TEXTURE_HANDLE);
	assert(m_Handles.contains(vHandle));
	glBindTextureUnit(vBindUnit, vHandle);
}

void CTextureManager::bindImage2D(TextureHandle vHandle, GLuint vBindUnit, GLenum vAccess, GLint vLevel) const
{
	// error when vFormat == GL_RGB8
	assert(vHandle != INVALID_TEXTURE_HANDLE);
	assert(m_Handles.contains(vHandle));
	GLint InternalFormat;
	glGetTextureLevelParameteriv(vHandle, vLevel, GL_TEXTURE_INTERNAL_FORMAT, &InternalFormat);
	glBindImageTexture(vBindUnit, vHandle, vLevel, GL_FALSE, 0, vAccess, InternalFormat);
}

void CTextureManager::deleteTexture(TextureHandle vHandle)
{
	assert(vHandle != INVALID_TEXTURE_HANDLE);
	assert(m_Handles.contains(vHandle));
	glDeleteTextures(1, &vHandle);
	m_Handles.erase(vHandle);
	if (m_HandleToPath.contains(vHandle))
	{
		m_PathToHandle.erase(m_HandleToPath.at(vHandle));
		m_HandleToPath.erase(vHandle);
	}
}

SExtent2D CTextureManager::queryTextureExtent(TextureHandle vHandle, GLint vLevel) const
{
	if (vHandle == INVALID_TEXTURE_HANDLE || !m_Handles.contains(vHandle)) return {0, 0};
	SExtent2D Extent;
	glGetTextureLevelParameteriv(vHandle, vLevel, GL_TEXTURE_WIDTH, &Extent._Width);
	glGetTextureLevelParameteriv(vHandle, vLevel, GL_TEXTURE_HEIGHT, &Extent._Height);
	return Extent;
}

GLint CTextureManager::queryTextureNumLevels(TextureHandle vHandle) const
{
	if (vHandle == INVALID_TEXTURE_HANDLE || !m_Handles.contains(vHandle)) return 0;
	int NumLevels;
	glGetTextureParameteriv(vHandle, GL_TEXTURE_IMMUTABLE_LEVELS, &NumLevels);
	return NumLevels;
}

void CTextureManager::cleanup()
{
	for (const std::vector Handles(m_Handles.begin(), m_Handles.end()); const auto Handle : Handles)
	{
		deleteTexture(Handle);
	}
	assert(m_Handles.empty());
	assert(m_HandleToPath.empty());
	assert(m_PathToHandle.empty());
}
