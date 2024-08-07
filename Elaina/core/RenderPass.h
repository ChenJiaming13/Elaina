#pragma once

#include <memory>
#include <vector>

namespace Elaina
{
	class CScene;
	class CFrameBuffer;
	class CShaderProgram;
	class CRenderPass
	{
	public:
		explicit CRenderPass(const std::shared_ptr<CShaderProgram>& vShaderProgram) :m_pShaderProgram(vShaderProgram) {}
		virtual ~CRenderPass() { m_pShaderProgram.reset(); }

		virtual void renderV(
			const std::shared_ptr<CScene>& vScene, 
			const std::vector<std::shared_ptr<CFrameBuffer>>& vFrameBuffers, 
			const std::vector<size_t>& vOutputIndices,
			size_t vIdxOfPasses
		);

	protected:
		std::shared_ptr<CShaderProgram> m_pShaderProgram;
	};
}