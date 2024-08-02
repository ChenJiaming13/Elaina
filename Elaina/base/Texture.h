#pragma once

namespace Elaina
{
	class CTexture
	{
	public:
		CTexture(GLenum vTextureType, int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType);
		virtual ~CTexture();

		virtual void resize(int vWidth, int vHeight) = 0;
		void bind() const;
		void setParameter(GLenum vName, GLint vValue) const;
		void setParameter(GLenum vName, const GLfloat* vValue) const;
		void generateMipmap() const;
		GLenum getTextureType() const { return m_TextureType; }
		GLuint getID() const { return m_TextureID; }
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }

	protected:
		GLenum m_TextureType;
		GLuint m_TextureID;
		GLenum m_InternalFormat;
		GLenum m_Format;
		GLenum m_DataType;
		int m_Width, m_Height;
	};
}