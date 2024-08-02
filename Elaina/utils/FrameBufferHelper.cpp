#include "pch.h"
#include "FrameBufferHelper.h"
#include "base/Framebuffer.h"
#include "base/Texture2D.h"
#include "base/TextureCube.h"

std::shared_ptr<Elaina::CFrameBuffer> Elaina::CFrameBufferHelper::createColorAndDepthFrameBuffer(int vWidth, int vHeight, const std::vector<int>& vColorBufferChannels)
{
	int NumOfColorBuffers = (int)vColorBufferChannels.size();
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
	pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex, 0);
	CFrameBuffer::setColorBufferEmpty();
	CFrameBuffer::checkComplete();
	pFrameBuffer->unbind();
	return pFrameBuffer;
}

std::shared_ptr<Elaina::CFrameBuffer> Elaina::CFrameBufferHelper::createPointLightShadowFrameBuffer(int vWidth, int vHeight)
{
	_ASSERTE(vWidth > 0 && vHeight > 0);

	const auto& pDepthTex = std::make_shared<CTextureCube>(vWidth, vHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	pDepthTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	pDepthTex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	const auto& pFrameBuffer = std::make_shared<CFrameBuffer>();
	pFrameBuffer->create();
	pFrameBuffer->bind();
	pFrameBuffer->setAttachment(GL_DEPTH_ATTACHMENT, pDepthTex, 0);
	CFrameBuffer::setColorBufferEmpty();
	CFrameBuffer::checkComplete();
	pFrameBuffer->unbind();
	return pFrameBuffer;
}
