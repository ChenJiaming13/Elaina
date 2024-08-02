#include "pch.h"
#include "Texture.h"
#include "safe.h"

Elaina::CTexture::CTexture(GLenum vTextureType, int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType)
	:m_TextureType(vTextureType), m_TextureID(0), m_InternalFormat(vInternalFormat), m_Format(vFormat), m_DataType(vDataType), m_Width(0), m_Height(0)
{
	GL_SAFE_CALL(glGenTextures(1, &m_TextureID));
}

Elaina::CTexture::~CTexture()
{
	GL_SAFE_CALL(glDeleteTextures(1, &m_TextureID));
}

void Elaina::CTexture::bind() const
{
	GL_SAFE_CALL(glBindTexture(m_TextureType, m_TextureID));
}

void Elaina::CTexture::setParameter(GLenum vName, GLint vValue) const
{
	GL_SAFE_CALL(glTexParameteri(m_TextureType, vName, vValue));
}

void Elaina::CTexture::setParameter(GLenum vName, const GLfloat* vValue) const
{
	GL_SAFE_CALL(glTexParameterfv(m_TextureType, vName, vValue));
}

void Elaina::CTexture::generateMipmap() const
{
	GL_SAFE_CALL(glGenerateMipmap(m_TextureType));
}

