#include "pch.h"
#include <ranges>
#include "FrameBuffer.h"
#include "Texture.h"
#include "RenderBuffer.h"
#include "safe.h"
#include "Texture2D.h"

namespace Elaina
{
	class CTexture2D;
}

Elaina::CFrameBuffer::CFrameBuffer()
	:m_FrameBufferID(0), m_TexturesMap{}, m_RenderBuffersMap{}, m_Width(0), m_Height(0)
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

void Elaina::CFrameBuffer::unbind(GLenum vTarget)
{
	GL_SAFE_CALL(glBindFramebuffer(vTarget, 0));
}

void Elaina::CFrameBuffer::resize(int vWidth, int vHeight)
{
	if (vWidth <= 0 || vHeight <= 0) return;
	if (m_Width == vWidth && m_Height == vHeight) return;
	m_Width = vWidth;
	m_Height = vHeight;
	for (const auto& val : m_TexturesMap | std::views::values)
		val->resize(vWidth, vHeight);
	for (const auto& val : m_RenderBuffersMap | std::views::values)
		val->resize(vWidth, vHeight);
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
	if (m_FrameBufferID == getDefaultFrameBuffer()->m_FrameBufferID)
	{
		spdlog::error("cannot get attachment from default framebuffer");
		throw std::runtime_error("get attachment failed");
	}
	return m_TexturesMap[vAttachmentType];
}

void Elaina::CFrameBuffer::setDrawAttachments(const std::vector<GLenum>& vAttachmentsType)
{
	GL_SAFE_CALL(glDrawBuffers(static_cast<GLsizei>(vAttachmentsType.size()), vAttachmentsType.data()));
}

bool Elaina::CFrameBuffer::checkComplete()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::warn("curr bind framebuffer is not complete!");
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
	static auto pDefaultFrameBuffer = std::make_shared<CFrameBuffer>();
	return pDefaultFrameBuffer;
}

void Elaina::CFrameBuffer::initDefaultFrameBuffer(int vWidth, int vHeight, GLuint vFrameBufferID)
{
	getDefaultFrameBuffer()->m_Width = vWidth;
	getDefaultFrameBuffer()->m_Height = vHeight;
	getDefaultFrameBuffer()->m_FrameBufferID = vFrameBufferID;
}

std::shared_ptr<Elaina::CFrameBuffer> Elaina::CFrameBuffer::createFrameBuffer(int vWidth, int vHeight,
	const std::vector<int>& vColorBufferChannels)
{
	const int NumOfColorBuffers = static_cast<int>(vColorBufferChannels.size());
	_ASSERTE(vWidth > 0 && vHeight > 0);
	_ASSERTE(NumOfColorBuffers > 0 && NumOfColorBuffers <= 32);

	const auto& pFrameBuffer = std::make_shared<CFrameBuffer>();
	pFrameBuffer->create();
	pFrameBuffer->bind();

	// color buffer
	std::vector<GLenum> DrawAttachments{};
	for (int i = 0; i < NumOfColorBuffers; ++i)
	{
		std::shared_ptr<CTexture2D> pTexture2D = nullptr;
		if (vColorBufferChannels[i] == 3)
		{
			pTexture2D = std::make_shared<CTexture2D>(vWidth, vHeight, GL_RGB16F, GL_RGB, GL_FLOAT);
		}
		else if (vColorBufferChannels[i] == 4)
		{
			pTexture2D = std::make_shared<CTexture2D>(vWidth, vHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		}
		else
		{
			spdlog::warn("curr only support channel == 3 or channel == 4");
			continue;
		}
		pTexture2D->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pTexture2D->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		pFrameBuffer->setAttachment(GL_COLOR_ATTACHMENT0 + i, pTexture2D, 0);
		DrawAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	pFrameBuffer->setDrawAttachments(DrawAttachments);

	// depth buffer
	const auto& pDepthTex = std::make_shared<CTexture2D>(vWidth, vHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	pDepthTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex, 0);

	// TODO: stencil buffer
	CFrameBuffer::checkComplete();
	CFrameBuffer::unbind();
	return pFrameBuffer;
}

void Elaina::CFrameBuffer::blit(
	const std::shared_ptr<CFrameBuffer>& vSrcFrameBuffer,
	const std::shared_ptr<CFrameBuffer>& vDstFrameBuffer, GLbitfield vMask, GLenum vFilter)
{

	vSrcFrameBuffer->bind(GL_READ_FRAMEBUFFER);
	vDstFrameBuffer->bind(GL_DRAW_FRAMEBUFFER);
	GL_SAFE_CALL(glBlitFramebuffer(
		0, 0, vSrcFrameBuffer->getWidth(), vSrcFrameBuffer->getHeight(),
		0, 0, vDstFrameBuffer->getWidth(), vDstFrameBuffer->getHeight(),
		vMask, vFilter
	));
	vSrcFrameBuffer->unbind(GL_READ_FRAMEBUFFER);
	vDstFrameBuffer->unbind(GL_DRAW_FRAMEBUFFER);
}
