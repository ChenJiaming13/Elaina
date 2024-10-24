#include "pch.h"
#include "Texture2D.h"
#include <stb_image.h>
#include "safe.h"

Elaina::CTexture2D::CTexture2D(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType)
	:CTexture(GL_TEXTURE_2D, vWidth, vHeight, vInternalFormat, vFormat, vDataType)
{
	resize(vWidth, vHeight);
}

Elaina::CTexture2D::CTexture2D(const std::string& vImageFile, bool vGenerateMipmap)
	:CTexture(GL_TEXTURE_2D, 0, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE)
{
	int Width = 0, Height = 0, NrChannels = 0;
	const unsigned char* pData = stbi_load(vImageFile.c_str(), &Width, &Height, &NrChannels, 0);
	if (pData == nullptr)
	{
		spdlog::error("failed to read image: {}", vImageFile);
	}
	if (Width == 0 || Height == 0 || NrChannels == 0)
	{
		spdlog::error("image is empty");
	}
	if (NrChannels == 3)
	{
		m_InternalFormat = m_Format = GL_RGB;
	}
	else if (NrChannels == 4)
	{
		m_InternalFormat = m_Format = GL_RGBA;
	}
	else
	{
		spdlog::error("only support RGB or RGBA format");
	}
	resize(Width, Height, pData);
	if (vGenerateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
}

void Elaina::CTexture2D::resize(int vWidth, int vHeight)
{
	resize(vWidth, vHeight, nullptr);
}

void Elaina::CTexture2D::resize(int vWidth, int vHeight, const GLvoid* vPixels)
{
	m_Width = vWidth;
	m_Height = vHeight;
	bind();
	GL_SAFE_CALL(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, vWidth, vHeight, 0, m_Format, m_DataType, vPixels));
}