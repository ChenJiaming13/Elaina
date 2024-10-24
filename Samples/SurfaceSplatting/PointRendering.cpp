#include "PointRendering.h"
#include "helper.h"
#include "safe.h"
#include "base/Framebuffer.h"
#include "base/ShaderProgram.h"
#include "core/Camera.h"

void CPointRendering::initV(int vWidth, int vHeight, const std::vector<SSurfel>& vSurfels)
{
	m_SurfelsSize = static_cast<GLsizei>(vSurfels.size());
	m_SurfelsVAO = __createVAO(vSurfels);
	m_pPointProgram = Elaina::CShaderProgram::createShaderProgram(
		"shaders/point.vert.glsl",
		"shaders/point.frag.glsl"
	);
	m_pDefaultFrameBuffer = Elaina::CFrameBuffer::getDefaultFrameBuffer();
}

void CPointRendering::onWindowSizeChangeV(int vWidth, int vHeight)
{
	m_pDefaultFrameBuffer->resize(vWidth, vHeight);
}

void CPointRendering::renderV(const std::shared_ptr<Elaina::CCamera>& vCamera)
{
	m_pDefaultFrameBuffer->bind();
	GL_SAFE_CALL(glViewport(0, 0, m_pDefaultFrameBuffer->getWidth(), m_pDefaultFrameBuffer->getHeight()));
	GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
	GL_SAFE_CALL(glDepthMask(GL_TRUE));
	static glm::vec4 BackgroundColor = glm::vec4(0.1f, 0.2f, 0.3f, 1.0f);
	GL_SAFE_CALL(glClearColor(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, BackgroundColor.a));
	GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_pPointProgram->use();
	m_pPointProgram->setUniform("uView", vCamera->getViewMatrix());
	m_pPointProgram->setUniform("uProjection", vCamera->getProjectionMatrix());
	GL_SAFE_CALL(glBindVertexArray(m_SurfelsVAO));
	GL_SAFE_CALL(glDrawArrays(GL_POINTS, 0, m_SurfelsSize));
}

void CPointRendering::cleanup()
{
	glDeleteVertexArrays(1, &m_SurfelsVAO);
	m_SurfelsVAO = 0;
	m_pPointProgram.reset();
	m_pDefaultFrameBuffer.reset();
}

GLuint CPointRendering::__createVAO(const std::vector<SSurfel>& vSurfels)
{
	GLuint VAO, VBO;
	GL_SAFE_CALL(glGenVertexArrays(1, &VAO));
	GL_SAFE_CALL(glGenBuffers(1, &VBO));
	GL_SAFE_CALL(glBindVertexArray(VAO));

	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(SSurfel) * vSurfels.size(), vSurfels.data(), GL_STATIC_DRAW));
	configSurfelVAOLayout(0, false);
	GL_SAFE_CALL(glBindVertexArray(0));

	return VAO;
}
