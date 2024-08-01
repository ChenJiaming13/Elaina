#pragma once

#include <unordered_map>
#include <set>
#include <memory>
#include <glad/glad.h>

namespace Elaina
{
	class CTexture2D;
	class CRenderBuffer;
	class CFrameBuffer
	{
	public:
		CFrameBuffer();
		~CFrameBuffer();

		void create();
		void bind(GLenum vTarget = GL_FRAMEBUFFER) const;
		void unbind(GLenum vTarget = GL_FRAMEBUFFER) const;
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		void resize(int vWidth, int vHeight);
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture2D>& vTexture2D, GLint vTextureLevel = 0);
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CRenderBuffer>& vRenderBuffer);
		const std::shared_ptr<CTexture2D>& getAttachment(GLenum vAttachmentType);
		void setDrawAttachments(const std::vector<GLenum>& vAttachmentsType) const;
		
		static bool checkComplete();
		static void setColorBufferEmpty();
		static const std::shared_ptr<CFrameBuffer>& getDefaultFrameBuffer();
		static void initDefaultFrameBuffer(int vWidth, int vHeight, GLuint vFrameBufferID = 0);
		static std::shared_ptr<CFrameBuffer> createFrameBuffer(
			int vWidth,
			int vHeight,
			int vNumOfColorBuffer,
			bool vNeedDepthBuffer,
			bool vNeedStencilBuffer
		);
		static std::shared_ptr<CFrameBuffer> createDepthOnlyFrameBuffer(int vWidth, int vHeight);
	
	private:
		GLuint m_FrameBufferID;
		std::unordered_map<GLenum, std::shared_ptr<CTexture2D>> m_TexturesMap;
		std::unordered_map<GLenum, std::shared_ptr<CRenderBuffer>> m_RenderBuffersMap;
		int m_Width;
		int m_Height;
	};
}