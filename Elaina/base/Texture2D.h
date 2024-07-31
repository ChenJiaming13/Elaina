#pragma once

namespace Elaina
{
	class CTexture2D
	{
	public:
		CTexture2D(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType);
		~CTexture2D();

		GLuint getID() const { return m_TextureID; }
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		void bind() const;
		void resize(int vWidth, int vHeight, const GLvoid* vPixels = nullptr);

		static void setParameters(GLenum vName, GLint vValue);
		static void setParameters(GLenum vName, const GLfloat* vValue);
		static void generateMipmap();

	private:
		GLuint m_TextureID;
		GLenum m_InternalFormat;
		GLenum m_Format;
		GLenum m_DataType;
		int m_Width, m_Height;
	};
}