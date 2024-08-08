#pragma once

#include "core/RenderPass.h"

namespace Elaina
{
	class CVertexArrayObject;
	class CDirShadowMapPass;
	class CFrameBuffer;
	class CShaderProgram;
	class CTexture;
	class CDeferredLitPass final : public CRenderPass
	{
	public:
		explicit CDeferredLitPass(bool vIsFinalPass);
		~CDeferredLitPass() override;

		void initV(int vWidth, int vHeight) override;
		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void onWindowSizeChangeV(int vWidth, int vHeight) override;

		void setGeoFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pGeoFrameBuffer = vFrameBuffer; }
		void setDirShadowMapTex(const std::shared_ptr<CTexture>& vTexture) { m_pDirShadowMapTex = vTexture; }
		void setPointShadowMapTex(const std::shared_ptr<CTexture>& vTexture) { m_pPointShadowMapTex = vTexture; }

		void setEnablePCF(bool vEnablePCF) { m_EnablePCF = vEnablePCF; }
		void setHalfSizePCF(int vHalfSizePCF)
		{
			if (vHalfSizePCF < 0) return;
			m_HalfSizePCF = vHalfSizePCF;
		}
		[[nodiscard]] bool getEnablePCF() const { return m_EnablePCF; }
		[[nodiscard]] int getHalfSizePCF() const { return m_HalfSizePCF; }
		[[nodiscard]] const auto& getFrameBuffer() const { return m_pFrameBuffer; }
		[[nodiscard]] bool validateV() const override;

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CVertexArrayObject> m_pQuadVAO;
		std::shared_ptr<CFrameBuffer> m_pFrameBuffer;
		bool m_IsFinalPass;
		bool m_EnablePCF;
		int m_HalfSizePCF;
		std::shared_ptr<CFrameBuffer> m_pGeoFrameBuffer = nullptr;
		std::shared_ptr<CTexture> m_pDirShadowMapTex = nullptr;
		std::shared_ptr<CTexture> m_pPointShadowMapTex = nullptr;
	};
}
