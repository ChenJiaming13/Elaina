#include "pch.h"
#include "Buffer.h"
#include "safe.h"

Elaina::CBuffer::CBuffer(EBufferType vBufferType): m_BufferType(vBufferType), m_BufferID(0)
{
	GL_SAFE_CALL(glGenBuffers(1, &m_BufferID));
}

Elaina::CBuffer::~CBuffer()
{
	GL_SAFE_CALL(glDeleteBuffers(1, &m_BufferID));
}

void Elaina::CBuffer::bind() const
{
	GL_SAFE_CALL(glBindBuffer((GLenum)m_BufferType, m_BufferID));
}

void Elaina::CBuffer::setBufferData(GLsizeiptr vBufferSize, const void* vData, GLenum vUsage) const
{
	GL_SAFE_CALL(glBufferData((GLenum)m_BufferType, vBufferSize, vData, vUsage));
}
