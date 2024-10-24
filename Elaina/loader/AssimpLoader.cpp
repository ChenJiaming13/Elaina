#include "pch.h"
#include "AssimpLoader.h"
#include "scene/Node.h"
#include "base/VertexArrayObject.h"

std::shared_ptr<Elaina::CNode> Elaina::CAssimpLoader::loadModel(const std::string& vModelPath)
{
	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(vModelPath,
		aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_OptimizeMeshes
		| aiProcess_FlipUVs
	);

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		spdlog::error("[ASSIMP] {}", Importer.GetErrorString());
		return nullptr;
	}

	m_DirPath = vModelPath.substr(0, vModelPath.find_last_of('/'));
	return __processNode(pScene->mRootNode, pScene);
}

std::shared_ptr<Elaina::CNode> Elaina::CAssimpLoader::__processNode(const aiNode* vNode, const aiScene* vScene)
{
	auto pNode = std::make_shared<CNode>();
	for (unsigned int i = 0; i < vNode->mNumMeshes; ++i)
	{
		const aiMesh* pMesh = vScene->mMeshes[vNode->mMeshes[i]];
		pNode->addMesh(__processMesh(pMesh, vScene));
	}
	for (unsigned int i = 0; i < vNode->mNumChildren; ++i)
	{
		pNode->addChild(__processNode(vNode->mChildren[i], vScene));
	}
	return pNode;
}

std::shared_ptr<Elaina::CMesh> Elaina::CAssimpLoader::__processMesh(const aiMesh* vMesh, const aiScene* vScene)
{
	return nullptr;
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CAssimpLoader::__processGeometry(const aiMesh* vMesh,
	const aiScene* vScene) const
{
	std::vector<SVertex> Vertices;
	std::vector<unsigned int> Indices;

	for (unsigned int i = 0; i < vMesh->mNumVertices; i++)
	{
		SVertex Vertex;
		glm::vec3 Vec3;

		// positions
		Vec3.x = vMesh->mVertices[i].x;
		Vec3.y = vMesh->mVertices[i].y;
		Vec3.z = vMesh->mVertices[i].z;
		Vertex._Position = Vec3;

		// normals
		if (vMesh->HasNormals())
		{
			Vec3.x = vMesh->mNormals[i].x;
			Vec3.y = vMesh->mNormals[i].y;
			Vec3.z = vMesh->mNormals[i].z;
			Vertex._Normal = Vec3;
		}

		// texture coordinates (only consider main uv)
		if (vMesh->mTextureCoords[0])
		{
			glm::vec2 Vec2;
			Vec2.x = vMesh->mTextureCoords[0][i].x;
			Vec2.y = vMesh->mTextureCoords[0][i].y;
			Vertex._TexCoords = Vec2;

			// tangent (only fill when exist uv)
			Vec3.x = vMesh->mTangents[i].x;
			Vec3.y = vMesh->mTangents[i].y;
			Vec3.z = vMesh->mTangents[i].z;
			Vertex._Tangent = Vec3;

			//// bitangent
			//vector.x = vMesh->mBitangents[i].x;
			//vector.y = vMesh->mBitangents[i].y;
			//vector.z = vMesh->mBitangents[i].z;
			//vertex.Bitangent = vector;
		}
		else Vertex._TexCoords = glm::vec2(0.0f, 0.0f);

		Vertices.push_back(Vertex);
	}
	std::vector<float> RawVertices(Vertices.size() * (sizeof(SVertex) / sizeof(float)));
	std::memcpy(RawVertices.data(), Vertices.data(), sizeof(float) * RawVertices.size());

	for (unsigned int i = 0; i < vMesh->mNumFaces; i++)
	{
		aiFace Face = vMesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
			Indices.push_back(Face.mIndices[j]);
	}

	std::vector<int> Layout;
	SVertex::getLayout(Layout);

	return CVertexArrayObject::createVAO(RawVertices, Indices, Layout, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::SMaterial> Elaina::CAssimpLoader::__processMaterial(const aiMesh* vMesh,
	const aiScene* vScene) const
{
	const aiMaterial* pMaterial = vScene->mMaterials[vMesh->mMaterialIndex];
	//pMaterial->GetTextureCount()
	return nullptr;
}
