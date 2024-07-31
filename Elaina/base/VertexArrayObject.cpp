#include "pch.h"
#include "VertexArrayObject.h"
#include "Buffer.h"
#include "safe.h"

Elaina::CVertexArrayObject::CVertexArrayObject()
	:m_VAO(0), m_VerticesCount(0), m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr), m_DrawMode{}, m_IndexType{}
{
	GL_SAFE_CALL(glGenVertexArrays(1, &m_VAO));
}

Elaina::CVertexArrayObject::~CVertexArrayObject()
{
	GL_SAFE_CALL(glDeleteVertexArrays(1, &m_VAO));
}

void Elaina::CVertexArrayObject::bind() const
{
	GL_SAFE_CALL(glBindVertexArray(m_VAO));
}

void Elaina::CVertexArrayObject::draw() const
{
	if (m_pIndexBuffer != nullptr)
	{
		GL_SAFE_CALL(glDrawElements(m_DrawMode, m_VerticesCount, m_IndexType, 0));
	}
	else
	{
		GL_SAFE_CALL(glDrawArrays(m_DrawMode, 0, m_VerticesCount));
	}
}

void Elaina::CVertexArrayObject::addBuffer(const std::shared_ptr<CBuffer>& vBuffer)
{
	if (vBuffer->getBufferType() == CBuffer::EBufferType::VERTEX_BUFFER)
		m_pVertexBuffer = vBuffer;
	else if (vBuffer->getBufferType() == CBuffer::EBufferType::INDEX_BUFFER)
		m_pIndexBuffer = vBuffer;
	else spdlog::warn("undefined buffer type");
}

void Elaina::CVertexArrayObject::setVertexLayout(const std::vector<int>& vLayout) const
{
	unsigned int TotalCount = std::accumulate(vLayout.begin(), vLayout.end(), 0);
	unsigned int CurrAccumCount = 0;
	for (int i = 0; i < vLayout.size(); ++i)
	{
		GL_SAFE_CALL(glVertexAttribPointer(i, vLayout[i], GL_FLOAT, GL_FALSE, TotalCount * sizeof(float), (void*)(CurrAccumCount * sizeof(float))));
		GL_SAFE_CALL(glEnableVertexAttribArray(i));
		CurrAccumCount += vLayout[i];
	}
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CVertexArrayObject::createVAO(const std::vector<float>& vVertices, const std::vector<int>& vLayout, GLenum vDrawMode, GLenum vUsage)
{
	const auto& pVAO = std::make_shared<CVertexArrayObject>();
	pVAO->bind();
	pVAO->setDrawMode(vDrawMode);
	//pVAO->setVerticesCount(vVertices.size() / std::accumulate(vLayout.begin(), vLayout.end(), 0));
	pVAO->setVerticesCount((GLsizei)vVertices.size());

	const auto& pVertexBuffer = std::make_shared<CBuffer>(CBuffer::EBufferType::VERTEX_BUFFER);
	pVertexBuffer->bind();
	pVertexBuffer->setBufferData(sizeof(float) * vVertices.size(), vVertices.data(), vUsage);
	pVAO->addBuffer(pVertexBuffer);
	pVAO->setVertexLayout(vLayout);

	return pVAO;
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CVertexArrayObject::createVAO(const std::vector<float>& vVertices, const std::vector<unsigned int>& vIndices, const std::vector<int>& vLayout, GLenum vDrawMode, GLenum vUsage)
{
	const auto& pVAO = createVAO(vVertices, vLayout, vDrawMode, vUsage);

	const auto& pIndexBuffer = std::make_shared<CBuffer>(CBuffer::EBufferType::INDEX_BUFFER);
	pIndexBuffer->bind();
	pIndexBuffer->setBufferData(sizeof(unsigned int) * vIndices.size(), vIndices.data(), vUsage);
	pVAO->addBuffer(pIndexBuffer);
	pVAO->setIndexType(GL_UNSIGNED_INT);

	return pVAO;
}