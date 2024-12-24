#include "pch.h"
#include "BufferManager.h"

using namespace Elaina::gl;

CBufferManager::~CBufferManager()
{
	std::vector<BufferHandle> Handles;
	for (const BufferHandle Handle : m_BufferInfos | std::views::keys)
	{
		Handles.push_back(Handle);
	}
	for (const BufferHandle Handle : Handles)
	{
		deleteBuffer(Handle);
	}
}

BufferHandle CBufferManager::createBuffer(const SBufferCreateInfo& vCreateInfo)
{
	BufferHandle Handle = INVALID_BUFFER_HANDLE;
	glCreateBuffers(1, &Handle);
	glNamedBufferStorage(Handle, vCreateInfo._BufferSize, vCreateInfo._pData, vCreateInfo._BufferUsage);
	//glNamedBufferData(Handle, vCreateInfo._BufferSize, vCreateInfo._pData, vCreateInfo._BufferUsage); // TODO
	if (Handle != INVALID_BUFFER_HANDLE) m_BufferInfos[Handle] = vCreateInfo;
	return Handle;
}

void CBufferManager::deleteBuffer(BufferHandle vHandle)
{
	assert(vHandle != INVALID_BUFFER_HANDLE);
	assert(m_BufferInfos.contains(vHandle));
	glDeleteBuffers(1, &vHandle);
	m_BufferInfos.erase(vHandle);
}
