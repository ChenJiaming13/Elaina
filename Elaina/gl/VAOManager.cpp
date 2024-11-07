#include "pch.h"
#include "VAOManager.h"

using namespace Elaina::gl;

VAOHandle CVAOManager::createVAO(const SVAOCreateInfo& vCreateInfo)
{
	if (vCreateInfo._VertexBufferHandle == INVALID_BUFFER_HANDLE ||
		vCreateInfo._IndexBufferHandle == INVALID_BUFFER_HANDLE ||
		vCreateInfo._Layouts.empty()) return INVALID_VAO_HANDLE;

	VAOHandle Handle = INVALID_VAO_HANDLE;
	glCreateVertexArrays(1, &Handle);

	const int TotalNumFloats = std::accumulate(vCreateInfo._Layouts.begin(), vCreateInfo._Layouts.end(), 0);
	constexpr auto BindingIndex = 0;
	glVertexArrayVertexBuffer(Handle, BindingIndex, vCreateInfo._VertexBufferHandle, 0, static_cast<GLsizei>(sizeof(float) * TotalNumFloats));
	glVertexArrayElementBuffer(Handle, vCreateInfo._IndexBufferHandle);

	int CurrNumFloats = 0;
	for (int AttrIndex = 0; AttrIndex < static_cast<int>(vCreateInfo._Layouts.size()); ++AttrIndex)
	{
		glEnableVertexArrayAttrib(Handle, AttrIndex);
		glVertexArrayAttribFormat(
			Handle, AttrIndex,
			vCreateInfo._Layouts[AttrIndex], GL_FLOAT, GL_FALSE,
			static_cast<GLuint>(sizeof(float) * CurrNumFloats)
		);
		glVertexArrayAttribBinding(Handle, AttrIndex, BindingIndex);
		CurrNumFloats += vCreateInfo._Layouts[AttrIndex];
	}
	if (Handle != INVALID_VAO_HANDLE) m_Handles.insert(Handle);
	return Handle;
}

void CVAOManager::deleteVAO(VAOHandle vHandle)
{
	assert(vHandle != INVALID_VAO_HANDLE);
	assert(m_Handles.contains(vHandle));
	glDeleteVertexArrays(1, &vHandle);
	m_Handles.erase(vHandle);
}

void CVAOManager::bindVAO(VAOHandle vHandle) const
{
	assert(vHandle != INVALID_VAO_HANDLE);
	assert(m_Handles.contains(vHandle));
	glBindVertexArray(vHandle);
}
