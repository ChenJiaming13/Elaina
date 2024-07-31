#pragma once

namespace Elaina
{
	class CRenderBuffer
	{
	public:
		CRenderBuffer(int vWidth, int vHeight, GLenum vInternalformat);
		~CRenderBuffer();

		GLuint getID() const { return m_RenderBufferID; }
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		void resize(int vWidth, int vHeight);

	private:
		GLuint m_RenderBufferID;
		int m_Width;
		int m_Height;
		GLenum m_Internalformat;
	};
}