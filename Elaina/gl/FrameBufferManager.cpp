#include "pch.h"
#include "FrameBufferManager.h"

using namespace Elaina::gl;

FrameBufferHandle CFrameBufferManager::createFrameBuffer(const SFrameBufferInfo& vInfo)
{
	FrameBufferHandle Handle = INVALID_FRAME_BUFFER_HANDLE;
	glCreateFramebuffers(1, &Handle);

	constexpr size_t MaxColorAttachmentsCount = GL_COLOR_ATTACHMENT31 - GL_COLOR_ATTACHMENT0 + 1;
	if (vInfo._ColorHandles.size() > MaxColorAttachmentsCount)
	{
		spdlog::warn(
			"the max count of color attachments is {}, but it is currently {}",
			MaxColorAttachmentsCount, vInfo._ColorHandles.size()
		);
	}
	for (size_t i = 0; i < std::min(MaxColorAttachmentsCount, vInfo._ColorHandles.size()); ++i)
	{
		const auto ColorHandle = vInfo._ColorHandles[i];
		if (ColorHandle == INVALID_TEXTURE_HANDLE)
		{
			spdlog::warn("color attachment {} is invalid", i);
			continue;
		}
		glNamedFramebufferTexture(Handle, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i), ColorHandle, 0);
	}
	if (vInfo._DepthHandle != INVALID_TEXTURE_HANDLE)
	{
		glNamedFramebufferTexture(Handle, GL_DEPTH_ATTACHMENT, vInfo._DepthHandle, 0);
	}
	if (glCheckNamedFramebufferStatus(Handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::error("frame buffer {} is not complete", Handle);
	}
	m_FrameBufferInfos[Handle] = vInfo;
	return Handle;
}

void CFrameBufferManager::bindFrameBuffer(FrameBufferHandle vHandle) const
{
	assert(vHandle != INVALID_FRAME_BUFFER_HANDLE);
	if (vHandle != DEFAULT_FRAME_BUFFER_HANDLE) assert(m_FrameBufferInfos.contains(vHandle));
	glBindFramebuffer(GL_FRAMEBUFFER, vHandle);
}

void CFrameBufferManager::deleteFrameBuffer(FrameBufferHandle vHandle)
{

	assert(vHandle != INVALID_FRAME_BUFFER_HANDLE);
	assert(m_FrameBufferInfos.contains(vHandle));
	glDeleteFramebuffers(1, &vHandle);
	m_FrameBufferInfos.erase(vHandle);
}

void CFrameBufferManager::cleanup()
{
	std::vector<FrameBufferHandle> Handles;
	for (const auto Handle : m_FrameBufferInfos | std::views::keys)
		Handles.push_back(Handle);
	for (const auto Handle : Handles)
		deleteFrameBuffer(Handle);
	assert(m_FrameBufferInfos.empty());
}
