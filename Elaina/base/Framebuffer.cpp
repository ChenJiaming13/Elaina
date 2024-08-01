#include "pch.h"
#include "FrameBuffer.h"
#include "Texture2D.h"
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
		Pair.second->resize(vWidth, vHeight, nullptr);
	for (const auto& Pair : m_RenderBuffersMap)
		Pair.second->resize(vWidth, vHeight);
}

void Elaina::CFrameBuffer::setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture2D>& vTexture2D, GLint vTextureLevel)
{
	GL_SAFE_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, vAttachmentType, GL_TEXTURE_2D, vTexture2D->getID(), vTextureLevel));
	m_TexturesMap[vAttachmentType] = vTexture2D;
	m_Width = vTexture2D->getWidth();
	m_Height = vTexture2D->getHeight();
}

void Elaina::CFrameBuffer::setAttachment(GLenum vAttachmentType, const std::shared_ptr<CRenderBuffer>& vRenderBuffer)
{
	GL_SAFE_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, vAttachmentType, GL_RENDERBUFFER, vRenderBuffer->getID()));
	m_RenderBuffersMap[vAttachmentType] = vRenderBuffer;
	m_Width = vRenderBuffer->getWidth();
	m_Height = vRenderBuffer->getHeight();
}

const std::shared_ptr<Elaina::CTexture2D>& Elaina::CFrameBuffer::getAttachment(GLenum vAttachmentType)
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

std::shared_ptr<Elaina::CFrameBuffer> Elaina::CFrameBuffer::createFrameBuffer(
	int vWidth,
	int vHeight,
	int vNumOfColorBuffer,
	bool vNeedDepthBuffer,
	bool vNeedStencilBuffer
)
{	
	_ASSERTE(vWidth > 0 && vHeight > 0);
	_ASSERTE(vNumOfColorBuffer >= 0 && vNumOfColorBuffer <= 32);

	const auto& pFrameBuffer = std::make_shared<CFrameBuffer>();
	pFrameBuffer->create();
	pFrameBuffer->bind();
	if (vNumOfColorBuffer > 0)
	{
		std::vector<GLenum> DrawAttachments{};
		for (int i = 0; i < vNumOfColorBuffer; ++i)
		{
			const auto& pTexture2D = std::make_shared<CTexture2D>(vWidth, vHeight, GL_RGB16F, GL_RGB, GL_FLOAT);
			pTexture2D->setParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			pTexture2D->setParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			pFrameBuffer->setAttachment(GL_COLOR_ATTACHMENT0 + i, pTexture2D);
			DrawAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		pFrameBuffer->setDrawAttachments(DrawAttachments);
	}
	else setColorBufferEmpty();
	// when color = 0 set none
	if (vNeedDepthBuffer)
	{
		const auto& pDepthTex = std::make_shared<CTexture2D>(vWidth, vHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
		pDepthTex->setParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pDepthTex->setParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex);
	}
	if (vNeedStencilBuffer) {} // TODO
	CFrameBuffer::checkComplete();
	pFrameBuffer->unbind();
	return pFrameBuffer;
}