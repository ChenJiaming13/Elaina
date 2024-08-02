#pragma once

#include "Texture.h"

namespace Elaina
{
	class CTextureCube final : public CTexture
	{
	public:
		CTextureCube(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType);
		explicit CTextureCube(
			const std::array<std::string, 6>& vCubeMapFiles,
			GLenum vInternalFormat = GL_RGB,
			GLenum vFormat = GL_RGB,
			GLenum vDataType = GL_UNSIGNED_BYTE);

		// Inherited via CTexture
		void resize(int vWidth, int vHeight) override;
		void resize(int vWidth, int vHeight, const std::array<const GLvoid*, 6>& vData);
	};
}