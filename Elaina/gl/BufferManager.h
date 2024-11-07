#pragma once

namespace Elaina::gl
{
	using BufferHandle = GLuint;

	constexpr auto INVALID_BUFFER_HANDLE = std::numeric_limits<BufferHandle>::max();

	struct SBufferCreateInfo
	{
		GLsizeiptr _BufferSize = 0;
		GLenum _BufferUsage = 0;
		const void* _pData = nullptr;
	};

	class CBufferManager
	{
	public:
		BufferHandle createBuffer(const SBufferCreateInfo& vCreateInfo);

		void deleteBuffer(BufferHandle vHandle);

	private:
		std::map<BufferHandle, SBufferCreateInfo> m_BufferInfos;
	};
}