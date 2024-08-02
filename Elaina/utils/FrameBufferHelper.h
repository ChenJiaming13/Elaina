#pragma once

namespace Elaina
{
	class CFrameBuffer;
	class CFrameBufferHelper
	{
	public:
		static std::shared_ptr<CFrameBuffer> createColorAndDepthFrameBuffer(int vWidth, int vHeight, const std::vector<int>& vColorBufferChannels);
		static std::shared_ptr<CFrameBuffer> createDepthOnlyFrameBuffer(int vWidth, int vHeight);
		static std::shared_ptr<CFrameBuffer> createPointLightShadowFrameBuffer(int vWidth, int vHeight);
	};
}
