#pragma once

#include <memory>

namespace Elaina
{
	class CScene;
	class IRenderPass
	{
	public:
		IRenderPass() = default;
		virtual ~IRenderPass() = default;

		virtual void renderV(const std::shared_ptr<CScene>& vScene) = 0;
		virtual void initV(int vWidth, int vHeight) {}
		virtual void onWindowSizeChangeV(int vWidth, int vHeight) {}
		[[nodiscard]] virtual bool validateV() const { return true; }
	};
}
