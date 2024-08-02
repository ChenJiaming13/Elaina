#include "pch.h"
#include "TextureCube.h"
#include "safe.h"

Elaina::CTextureCube::CTextureCube(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType)
	:CTexture(GL_TEXTURE_CUBE_MAP, vWidth, vHeight, vInternalFormat, vFormat, vDataType)
{
	resize(vWidth, vHeight);
}

Elaina::CTextureCube::CTextureCube(const std::array<std::string, 6>& vCubeMapFiles, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType)
	:CTexture(GL_TEXTURE_CUBE_MAP, 0, 0, vInternalFormat, vFormat, vDataType)
{
	int Width = 0, Height = 0, NrChannels = 0;
	std::array<unsigned char*, 6> pRawDatas{};
	std::array<const GLvoid*, 6> pDatas{};
	for (size_t i = 0; i < pRawDatas.size(); ++i)
	{
		unsigned char* pData = stbi_load(vCubeMapFiles[i].c_str(), &Width, &Height, &NrChannels, 0);
		if (pData == nullptr)
		{
			spdlog::warn("cubemap {} texture failed to load at path: {}", i, vCubeMapFiles[i]);
		}
		pRawDatas[i] = pData;
		pDatas[i] = (const GLvoid*)pData;
	}

	if (Width != 0 && Height != 0 && NrChannels != 0) resize(Width, Height, pDatas);
	
	for (size_t i = 0; i < pRawDatas.size(); ++i)
	{
		stbi_image_free(pRawDatas[i]);
	}
}

void Elaina::CTextureCube::resize(int vWidth, int vHeight)
{
	resize(vWidth, vHeight, {});
}

void Elaina::CTextureCube::resize(int vWidth, int vHeight, const std::array<const GLvoid*, 6>& vData)
{
	m_Width = vWidth;
	m_Height = vHeight;
	bind();
	for (GLuint i = 0; i < 6; ++i)
	{
		GL_SAFE_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, vWidth, vHeight, 0, m_Format, m_DataType, vData[i]));
	}
}
