#include "pch.h"
#include "GeometryManager.h"

using namespace Elaina::gl;

CGeometryManager::~CGeometryManager()
{
	std::vector<GeometryHandle> Handles;
	for (const auto& Handle : m_GeometryInfos | std::views::keys)
	{
		Handles.push_back(Handle);
	}
	for (const BufferHandle Handle : Handles)
	{
		deleteGeometry(Handle);
	}
}

GeometryHandle CGeometryManager::createGeometry(const SGeometryInfo& vInfo)
{
	if (vInfo._VertexBufferHandle == INVALID_BUFFER_HANDLE || vInfo._Layouts.empty()) return INVALID_GEOMETRY_HANDLE;

	GeometryHandle Handle = INVALID_GEOMETRY_HANDLE;
	glCreateVertexArrays(1, &Handle);

	if (vInfo._IndexBufferHandle != INVALID_BUFFER_HANDLE)
	{
		glVertexArrayElementBuffer(Handle, vInfo._IndexBufferHandle);
	}

	const int TotalNumFloats = std::accumulate(vInfo._Layouts.begin(), vInfo._Layouts.end(), 0);
	constexpr auto BindingIndex = 0;
	glVertexArrayVertexBuffer(Handle, BindingIndex, vInfo._VertexBufferHandle, 0, static_cast<GLsizei>(sizeof(float) * TotalNumFloats));

	int CurrNumFloats = 0;
	for (int AttrIndex = 0; AttrIndex < static_cast<int>(vInfo._Layouts.size()); ++AttrIndex)
	{
		glEnableVertexArrayAttrib(Handle, AttrIndex);
		glVertexArrayAttribFormat(
			Handle, AttrIndex,
			vInfo._Layouts[AttrIndex], GL_FLOAT, GL_FALSE,
			static_cast<GLuint>(sizeof(float) * CurrNumFloats)
		);
		glVertexArrayAttribBinding(Handle, AttrIndex, BindingIndex);
		CurrNumFloats += vInfo._Layouts[AttrIndex];
	}
	if (Handle != INVALID_GEOMETRY_HANDLE) m_GeometryInfos[Handle] = vInfo;
	return Handle;
}

void CGeometryManager::deleteGeometry(GeometryHandle vHandle)
{
	assert(vHandle != INVALID_GEOMETRY_HANDLE);
	assert(m_GeometryInfos.contains(vHandle));
	glDeleteVertexArrays(1, &vHandle);
	m_GeometryInfos.erase(vHandle);
}

void CGeometryManager::draw(GeometryHandle vHandle) const
{
	assert(vHandle != INVALID_GEOMETRY_HANDLE);
	assert(m_GeometryInfos.contains(vHandle));
	const auto& Geometry = m_GeometryInfos.at(vHandle);
	glBindVertexArray(vHandle);
	if (Geometry._IndexBufferHandle == INVALID_BUFFER_HANDLE)
	{
		const int TotalNumFloats = std::accumulate(Geometry._Layouts.begin(), Geometry._Layouts.end(), 0);
		glDrawArrays(Geometry._Primitive, 0, static_cast<GLsizei>(Geometry._VerticesCount * TotalNumFloats));
	}
	else
	{
		glDrawElements(Geometry._Primitive, static_cast<GLsizei>(Geometry._IndicesCount), GL_UNSIGNED_INT, nullptr);
	}
}
