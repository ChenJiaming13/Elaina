#pragma once

#include <glad/glad.h>
#include "RenderAlgorithm.h"

namespace Elaina
{
	class CFrameBuffer;
	class CShaderProgram;
}

class CPointRendering final : public CRenderAlgorithm
{
public:
	[[nodiscard]] std::string getName() const override { return "Point Rendering"; }
	void initV(int vWidth, int vHeight, const std::vector<SSurfel>& vSurfels) override;
	void onWindowSizeChangeV(int vWidth, int vHeight) override;
	void renderV(const std::shared_ptr<Elaina::CCamera>& vCamera) override;
	void cleanup() override;

private:
	static GLuint __createVAO(const std::vector<SSurfel>& vSurfels);

	GLuint m_SurfelsVAO = 0;
	GLsizei m_SurfelsSize = 0;
	std::shared_ptr<Elaina::CFrameBuffer> m_pDefaultFrameBuffer = nullptr;
	std::shared_ptr<Elaina::CShaderProgram> m_pPointProgram = nullptr;
};

