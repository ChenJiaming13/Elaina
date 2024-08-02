#include "pch.h"
#include "Texture2D.h"
#include "safe.h"

Elaina::CTexture2D::CTexture2D(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType)
	:CTexture(GL_TEXTURE_2D, vWidth, vHeight, vInternalFormat, vFormat, vDataType)
{
	resize(vWidth, vHeight);
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