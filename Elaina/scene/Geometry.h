#pragma once

namespace Elaina
{
	struct SVertex
	{
		glm::vec3 _Position;
		glm::vec3 _Normal;
		glm::vec2 _TexCoord;
	};

	class CGeometry
	{
	public:
		CGeometry(const std::vector<SVertex>& vVertices, const std::vector<unsigned int>& vIndices)
			:m_Vertices(vVertices), m_Indices(vIndices) {}

	private:
		std::vector<SVertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
	};
}