#include "pch.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "RenderBuffer.h"
#include "safe.h"

Elaina::CFrameBuffer::CFrameBuffer()
	:m_FrameBufferID(0), m_Width(0), m_Height(0), m_TexturesMap{}, m_RenderBuffersMap{}
{
}

Elaina::CFrameBuffer::~CFrameBuffer()
{
	if (m_FrameBufferID != getDefaultFrameBuffer()->m_FrameBufferID)
		GL_SAFE_CALL(glDeleteFramebuffers(1, &m_FrameBufferID));
}

void Elaina::CFrameBuffer::create()
{
	GL_SAFE_CALL(glGenFramebuffers(1, &m_FrameBufferID));
}

void Elaina::CFrameBuffer::bind(GLenum vTarget) const
{
	GL_SAFE_CALL(glBindFramebuffer(vTarget, m_FrameBufferID));
}

void Elaina::CFrameBuffer::unbind(GLenum vTarget) const
{
	GL_SAFE_CALL(glBindFramebuffer(vTarget, 0));
}

void Elaina::CFrameBuffer::resize(int vWidth, int vHeight)
{
	m_Width = vWidth;
	m_Height = vHeight;
	for (const auto& Pair : m_TexturesMap)
		Pair.second->resize(vWidth, vHeight);
	for (const auto& Pair : m_RenderBuffersMap)
		Pair.second->resize(vWidth, vHeight);
}

void Elaina::CFrameBuffer::setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture>& vTexture, GLint vTextureLevel)
{
	if (vTexture->getTextureType() == GL_TEXTURE_2D)
	{
		GL_SAFE_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, vAttachmentType, vTexture->getTextureType(), vTexture->getID(), vTextureLevel));
	}
	else if (vTexture->getTextureType() == GL_TEXTURE_CUBE_MAP)
	{
		GL_SAFE_CALL(glFramebufferTexture(GL_FRAMEBUFFER, vAttachmentType, vTexture->getID(), vTextureLevel));
	}
	else
	{
		spdlog::warn("frame buffer curr not support this texture type: {}", vTexture->getTextureType());
		return;
	}
	m_TexturesMap[vAttachmentType] = vTexture;
	m_Width = vTexture->getWidth();
	m_Height = vTexture->getHeight();
}

void Elaina::CFrameBuffer::setAttachment(GLenum vAttachmentType, const std::shared_ptr<CRenderBuffer>& vRenderBuffer)
{
	GL_SAFE_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, vAttachmentType, GL_RENDERBUFFER, vRenderBuffer->getID()));
	m_RenderBuffersMap[vAttachmentType] = vRenderBuffer;
	m_Width = vRenderBuffer->getWidth();
	m_Height = vRenderBuffer->getHeight();
}

const std::shared_ptr<Elaina::CTexture>& Elaina::CFrameBuffer::getAttachment(GLenum vAttachmentType)
{
	return m_TexturesMap[vAttachmentType];
}

void Elaina::CFrameBuffer::setDrawAttachments(const std::vector<GLenum>& vAttachmentsType) const
{
	GL_SAFE_CALL(glDrawBuffers((GLsizei)vAttachmentsType.size(), vAttachmentsType.data()));
}

bool Elaina::CFrameBuffer::checkComplete()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::warn("curr binded framebuffer is not complete!");
		return false;
	}
	return true;
}

void Elaina::CFrameBuffer::setColorBufferEmpty()
{
	GL_SAFE_CALL(glDrawBuffer(GL_NONE));
	GL_SAFE_CALL(glReadBuffer(GL_NONE));
}

const std::shared_ptr<Elaina::CFrameBuffer>& Elaina::CFrameBuffer::getDefaultFrameBuffer()
{
	static std::shared_ptr<CFrameBuffer> pDefaultFrameBuffer = std::make_shared<CFrameBuffer>();
	return pDefaultFrameBuffer;
}

void Elaina::CFrameBuffer::initDefaultFrameBuffer(int vWidth, int vHeight, GLuint vFrameBufferID)
{
	getDefaultFrameBuffer()->m_Width = vWidth;
	getDefaultFrameBuffer()->m_Height = vHeight;
	getDefaultFrameBuffer()->m_FrameBufferID = vFrameBufferID;
}
