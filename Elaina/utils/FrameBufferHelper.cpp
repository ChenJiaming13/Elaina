#include "pch.h"
#include "FrameBufferHelper.h"
#include "base/Framebuffer.h"
#include "base/Texture2D.h"
#include "base/TextureCube.h"

std::shared_ptr<Elaina::CFrameBuffer> Elaina::CFrameBufferHelper::createFrameBuffer(
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
			pTexture2D->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			pTexture2D->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			pFrameBuffer->setAttachment(GL_COLOR_ATTACHMENT0 + i, pTexture2D);
			DrawAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		pFrameBuffer->setDrawAttachments(DrawAttachments);
	}
	else CFrameBuffer::setColorBufferEmpty();
	// when color = 0 set none
	if (vNeedDepthBuffer)
	{
		const auto& pDepthTex = std::make_shared<CTexture2D>(vWidth, vHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
		pDepthTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pDepthTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex);
	}
	if (vNeedStencilBuffer) {} // TODO
	CFrameBuffer::checkComplete();
	pFrameBuffer->unbind();
	return pFrameBuffer;
}

std::shared_ptr<Elaina::CFrameBuffer> Elaina::CFrameBufferHelper::createDepthOnlyFrameBuffer(int vWidth, int vHeight)
{
	_ASSERTE(vWidth > 0 && vHeight > 0);

	const auto& pDepthTex = std::make_shared<CTexture2D>(vWidth, vHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	pDepthTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat BorderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	pDepthTex->setParameter(GL_TEXTURE_BORDER_COLOR, BorderColor);

	const auto& pFrameBuffer = std::make_shared<CFrameBuffer>();
	pFrameBuffer->create();
	pFrameBuffer->bind();
	pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex);
	CFrameBuffer::setColorBufferEmpty();
	CFrameBuffer::checkComplete();
	pFrameBuffer->unbind();
	return pFrameBuffer;
}
