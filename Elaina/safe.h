#pragma once

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#ifdef _DEBUG
inline void checkGLError()
{
	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR)
	{
		spdlog::error(__FUNCTION__);
	}
}

#define GL_SAFE_CALL(vCall) \
do { \
	vCall; \
	checkGLError(); \
} while(0)

#else
#define GL_SAFE_CALL(vCall) vCall
#endif