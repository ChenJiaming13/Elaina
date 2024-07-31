#include "pch.h"
#include "Texture2D.h"
#include "safe.h"

Elaina::CTexture2D::CTexture2D(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType)
	:m_TextureID(0), m_InternalFormat(vInternalFormat), m_Format(vFormat), m_DataType(vDataType), m_Width(0), m_Height(0)
{
	GL_SAFE_CALL(glGenTextures(1, &m_TextureID));
	resize(vWidth, vHeight, nullptr);
}

Elaina::CTexture2D::~CTexture2D()
{
	GL_SAFE_CALL(glDeleteTextures(1, &m_TextureID));
}

void Elaina::CTexture2D::bind() const
{
	GL_SAFE_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Elaina::CTexture2D::resize(int vWidth, int vHeight, const GLvoid* vPixels)
{
	m_Width = vWidth;
	m_Height = vHeight;
	bind();
	GL_SAFE_CALL(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, vWidth, vHeight, 0, m_Format, m_DataType, vPixels));
}

void Elaina::CTexture2D::setParameters(GLenum vName, GLint vValue)
{
	GL_SAFE_CALL(glTexParameteri(GL_TEXTURE_2D, vName, vValue));
}

void Elaina::CTexture2D::setParameters(GLenum vName, const GLfloat* vValue)
{
	GL_SAFE_CALL(glTexParameterfv(GL_TEXTURE_2D, vName, vValue));
}

void Elaina::CTexture2D::generateMipmap()
{
	GL_SAFE_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}