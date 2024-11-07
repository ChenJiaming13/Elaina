#pragma once

#include "TextureManager.h"

namespace Elaina::gl
{
	using FrameBufferHandle = GLuint;

	constexpr auto INVALID_FRAME_BUFFER_HANDLE = std::numeric_limits<FrameBufferHandle>::max();
	constexpr auto DEFAULT_FRAME_BUFFER_HANDLE = 0;

	struct SFrameBufferInfo
	{
		std::vector<TextureHandle> _ColorHandles{};
		TextureHandle _DepthHandle = INVALID_TEXTURE_HANDLE;
	};

	class CFrameBufferManager
	{
	public:
		~CFrameBufferManager() { cleanup(); }

		FrameBufferHandle createFrameBuffer(const SFrameBufferInfo& vInfo);

		void bindFrameBuffer(FrameBufferHandle vHandle) const;

		void deleteFrameBuffer(FrameBufferHandle vHandle);

		void cleanup();

	private:
		std::map<FrameBufferHandle, SFrameBufferInfo> m_FrameBufferInfos;
	};
}
