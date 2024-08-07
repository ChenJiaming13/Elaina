#pragma once

#include <unordered_map>
#include <set>
#include <memory>
#include <glad/glad.h>

namespace Elaina
{
	class CTexture;
	class CRenderBuffer;
	class CFrameBuffer
	{
	public:
		CFrameBuffer();
		~CFrameBuffer();

		void create();
		void bind(GLenum vTarget = GL_FRAMEBUFFER) const;
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		void resize(int vWidth, int vHeight);
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture>& vTexture, GLint vTextureLevel = 0);
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CRenderBuffer>& vRenderBuffer);
		const std::shared_ptr<CTexture>& getAttachment(GLenum vAttachmentType);

		static void unbind(GLenum vTarget = GL_FRAMEBUFFER);
		static void setDrawAttachments(const std::vector<GLenum>& vAttachmentsType);
		static bool checkComplete();
		static void setColorBufferEmpty();
		static const std::shared_ptr<CFrameBuffer>& getDefaultFrameBuffer();
		static void initDefaultFrameBuffer(int vWidth, int vHeight, GLuint vFrameBufferID = 0);
	
	private:
		GLuint m_FrameBufferID;
		std::unordered_map<GLenum, std::shared_ptr<CTexture>> m_TexturesMap;
		std::unordered_map<GLenum, std::shared_ptr<CRenderBuffer>> m_RenderBuffersMap;
		int m_Width;
		int m_Height;
	};
}