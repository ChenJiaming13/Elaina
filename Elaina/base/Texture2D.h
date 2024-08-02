#pragma once

#include "Texture.h"

namespace Elaina
{
	class CTexture2D final : public CTexture
	{
	public:
		CTexture2D(int vWidth, int vHeight, GLenum vInternalFormat, GLenum vFormat, GLenum vDataType);
		
		// Inherited via CTexture
		// TODO: error occurred when vWidth != vHeight
		void resize(int vWidth, int vHeight) override;
		void resize(int vWidth, int vHeight, const GLvoid* vPixels);
	};
}