#pragma once

namespace Elaina
{
	class CTextureCube
	{
	public:
		CTextureCube();
		~CTextureCube();

		void bind() const;
		bool loadCubeMapFiles(const std::vector<std::string>& vCubeMapFiles) const;

		static void setParameters(GLenum vName, GLint vValue);

	private:
		GLuint m_TextureID;
	};
}