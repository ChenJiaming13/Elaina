#pragma once

#include <glad/glad.h>

#ifdef _DEBUG
inline void checkGLError()
{
	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR)
	{
		throw Error;
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