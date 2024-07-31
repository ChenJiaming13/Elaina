#pragma once

namespace Elaina
{
	class CBuffer
	{
	public:
		enum class EBufferType
		{
			VERTEX_BUFFER = GL_ARRAY_BUFFER,
			INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER
		};

		CBuffer(EBufferType vBufferType);
		~CBuffer();

		void bind() const;
		EBufferType getBufferType() const { return m_BufferType; }
		void setBufferData(GLsizeiptr vBufferSize, const void* vData, GLenum vUsage = GL_STATIC_DRAW) const;

	private:
		EBufferType m_BufferType;
		GLuint m_BufferID;
	};
}