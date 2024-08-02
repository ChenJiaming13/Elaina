#pragma once

namespace Elaina
{
	class CFrameBuffer;
	class CFrameBufferHelper
	{
	public:
		static std::shared_ptr<CFrameBuffer> createFrameBuffer(
			int vWidth,
			int vHeight,
			int vNumOfColorBuffer,
			bool vNeedDepthBuffer,
			bool vNeedStencilBuffer
		);
		static std::shared_ptr<CFrameBuffer> createDepthOnlyFrameBuffer(int vWidth, int vHeight);
	};
}
