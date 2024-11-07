#pragma once

#include <set>
#include <vector>

#include "BufferManager.h"

namespace Elaina::gl
{
	using VAOHandle = GLuint;

	constexpr auto INVALID_VAO_HANDLE = std::numeric_limits<VAOHandle>::max();

	struct SVAOCreateInfo
	{
		BufferHandle _VertexBufferHandle = INVALID_BUFFER_HANDLE;
		BufferHandle _IndexBufferHandle = INVALID_BUFFER_HANDLE;
		std::vector<int> _Layouts{};
	};

	class CVAOManager
	{
	public:
		VAOHandle createVAO(const SVAOCreateInfo& vCreateInfo); // ABCABC

		void deleteVAO(VAOHandle vHandle);

		void bindVAO(VAOHandle vHandle) const;

	private:
		std::set<VAOHandle> m_Handles;
	};
}
