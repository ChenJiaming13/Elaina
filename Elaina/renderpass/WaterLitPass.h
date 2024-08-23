#pragma once
#include "core/RenderPass.h"

namespace Elaina
{
	class CShaderProgram;
	class CFrameBuffer;
	class CVertexArrayObject;
	class CTexture2D;
	class CWaterLitPass final : public CRenderPass
	{
	public:
		float _MoveSpeed = 0.03f;
		float _Tilling = 6.0f;
		float _WaveLength = 0.02f;
		glm::vec3 _WaterColor = glm::vec3(0.0f, 0.3f, 0.5f);

		CWaterLitPass();

		void renderV(const std::shared_ptr<CScene>& vScene) override;
		void setLitFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pLitFrameBuffer = vFrameBuffer; }
		void setReflectFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pReflectFrameBuffer = vFrameBuffer; }
		void setRefractFrameBuffer(const std::shared_ptr<CFrameBuffer>& vFrameBuffer) { m_pRefractFrameBuffer = vFrameBuffer; }
		[[nodiscard]] bool validateV() const override;
		[[nodiscard]] float getWaterHeight() const { return m_WaterHeight; }
		[[nodiscard]] glm::vec4 getReflectClipPlane() const { return {0.0f, 1.0f, 0.0f, -m_WaterHeight}; }
		[[nodiscard]] glm::vec4 getRefractClipPlane() const { return {0.0f, -1.0f, 0.0f, m_WaterHeight}; }

	private:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
		std::shared_ptr<CFrameBuffer> m_pLitFrameBuffer;
		std::shared_ptr<CFrameBuffer> m_pReflectFrameBuffer;
		std::shared_ptr<CFrameBuffer> m_pRefractFrameBuffer;
		std::shared_ptr<CVertexArrayObject> m_pWaterPlaneVAO;
		std::shared_ptr<CTexture2D> m_pDuDvMap;
		glm::mat4 m_ModelMatrix;
		float m_WaterHeight;
	};
}
