#include "pch.h"
#include "TextureCube.h"
#include "safe.h"

Elaina::CTextureCube::CTextureCube()
{
	GL_SAFE_CALL(glGenTextures(1, &m_TextureID));
}

Elaina::CTextureCube::~CTextureCube()
{
	GL_SAFE_CALL(glDeleteTextures(1, &m_TextureID));
}

void Elaina::CTextureCube::bind() const
{
	GL_SAFE_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID));
}

bool Elaina::CTextureCube::loadCubeMapFiles(const std::vector<std::string>& vCubeMapFiles) const
{
	bind();
	for (unsigned int i = 0; i < vCubeMapFiles.size(); i++)
	{
		int Width, Height, NrChannels;
		unsigned char* pData = stbi_load(vCubeMapFiles[i].c_str(), &Width, &Height, &NrChannels, 0);
		if (pData)
		{
			GL_SAFE_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData
			));
			stbi_image_free(pData);
		}
		else
		{
			spdlog::error("cubemap texture failed to load at path: {}", vCubeMapFiles[i]);
			stbi_image_free(pData);
			return false;
		}
	}
	setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	setParameters(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setParameters(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setParameters(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return true;
}

void Elaina::CTextureCube::setParameters(GLenum vName, GLint vValue)
{
	GL_SAFE_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, vName, vValue));
}


