#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace Elaina
{
	class CRenderPass;
	class CScene;
	class CRenderPipeline
	{
	public:
		CRenderPipeline() = default;

		void init(int vWidth, int vHeight) const;
		void onWindowSizeChange(int vWidth, int vHeight) const;
		void render(const std::shared_ptr<CScene>& vScene) const;
		void addRenderPass(
			const std::shared_ptr<CRenderPass>& vRenderPass,
			const std::function<void(const std::shared_ptr<CScene>& vScene)>& vFuncPrevRender = nullptr,
			const std::function<void(const std::shared_ptr<CScene>& vScene)>& vFuncPostRender = nullptr
		);
		void validate() const;
		void setPassEnable(size_t vIdxOfPasses, bool vEnable) { m_EnablePasses[vIdxOfPasses] = vEnable; }
		[[nodiscard]] bool getPassEnable(size_t vIdxOfPasses) const { return m_EnablePasses[vIdxOfPasses]; }

	private:
		std::vector<std::shared_ptr<CRenderPass>> m_RenderPasses;
		std::vector<bool> m_EnablePasses;
		std::vector<std::function<void(const std::shared_ptr<CScene>& vScene)>> m_FuncPrevRenders;
		std::vector<std::function<void(const std::shared_ptr<CScene>& vScene)>> m_FuncPostRenders;
	};
}