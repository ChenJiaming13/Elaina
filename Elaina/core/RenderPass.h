#pragma once

#include <memory>

namespace Elaina
{
	class CScene;
	class CRenderPass
	{
	public:
		CRenderPass() = default;
		virtual ~CRenderPass() = default;

		virtual void renderV(const std::shared_ptr<CScene>& vScene) = 0;
		virtual void initV(int vWidth, int vHeight) {}
		virtual void onWindowSizeChangeV(int vWidth, int vHeight) {}
	};
}
