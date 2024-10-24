#pragma once

struct aiNode;
struct aiScene;

namespace Elaina
{
	class CNode;
	class CMesh;
	struct SMaterial;
	class CVertexArrayObject;
	class CAssimpLoader
	{
	public:
		std::shared_ptr<CNode> loadModel(const std::string& vModelPath);

	private:
		struct SVertex
		{
			glm::vec3 _Position;
			glm::vec3 _Normal;
			glm::vec2 _TexCoords;
			glm::vec3 _Tangent;

			static void getLayout(std::vector<int>& voLayout)
			{
				voLayout = { 3,3,2,3 };
			}
		};

		std::shared_ptr<CNode> __processNode(const aiNode* vNode, const aiScene* vScene);
		std::shared_ptr<CMesh> __processMesh(const aiMesh* vMesh, const aiScene* vScene);
		std::shared_ptr<CVertexArrayObject> __processGeometry(const aiMesh* vMesh, const aiScene* vScene) const;
		std::shared_ptr<SMaterial> __processMaterial(const aiMesh* vMesh, const aiScene* vScene) const;

		std::string m_DirPath;
	};
}