#pragma once

#include <vector>
#include <glad/glad.h>

namespace Elaina
{
	class CBuffer;
	class CVertexArrayObject
	{
	public:
		CVertexArrayObject();
		~CVertexArrayObject();

		void bind() const;
		void draw() const;
		void addBuffer(const std::shared_ptr<CBuffer>& vBuffer);
		void setDrawMode(GLenum vDrawMode) { m_DrawMode = vDrawMode; }
		void setVerticesCount(GLsizei vVerticesCount) { m_VerticesCount = vVerticesCount; }
		void setIndexType(GLenum vIndexType) { m_IndexType = vIndexType; }

		static void setVertexLayout(const std::vector<int>& vLayout);
		static std::shared_ptr<CVertexArrayObject> createVAO(
			const std::vector<float>& vVertices,
			const std::vector<int>& vLayout,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);
		static std::shared_ptr<CVertexArrayObject> createVAO(
			const std::vector<float>& vVertices,
			const std::vector<unsigned int>& vIndices,
			const std::vector<int>& vLayout,
			GLenum vDrawMode = GL_TRIANGLES,
			GLenum vUsage = GL_STATIC_DRAW
		);

	private:
		GLuint m_VAO;
		GLsizei m_VerticesCount;
		GLenum m_DrawMode;
		GLenum m_IndexType;
		std::shared_ptr<CBuffer> m_pVertexBuffer;
		std::shared_ptr<CBuffer> m_pIndexBuffer;
	};
}