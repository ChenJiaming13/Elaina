#include "pch.h"
#include "ModelLoader.h"
#include "base/VertexArrayObject.h"
#include "core/Mesh.h"
#include "core/Node.h"

#define MODE_DEFAULT -1 
#define MODE_POINT 0
#define MODE_TRIANGLE 4 
#define COMPONETTYPE_UNSHORT 5123
#define COMPONETTYPE_UNINT   5125

bool loadGLTF(const std::string& vFilename, tinygltf::Model& vModelGLTF)
{
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool res = loader.LoadASCIIFromFile(&vModelGLTF, &err, &warn, vFilename);

	if (!warn.empty()) {
		spdlog::warn("WARN: {}", warn);
	}

	if (!err.empty()) {
		spdlog::error("ERR: {}", err);
	}

	if (!res) {
		spdlog::error("Failed to load glTF: {}", vFilename);
	}
	else {
		spdlog::info("Loaded glTF: {}", vFilename);
	}

	return res;
}

void createVertexBufferData(std::vector<float>& vVertices, const tinygltf::Buffer& vBuffer, const int vIndex) {
	float tempVertice;
	const int floatByte = 4;
	const int floatNum = 3;
	for (auto i = vIndex; i < vIndex + floatNum * floatByte; i += floatByte) {
		std::memcpy(&tempVertice, &vBuffer.data.at(i), sizeof(float));
		vVertices.push_back(tempVertice);
	}
}

void createIndiceBufferData(std::vector<unsigned int>& vIndices, const tinygltf::BufferView& vBufferView, const tinygltf::Buffer& vBuffer, const int& vComponentType) {
	unsigned short tempUShortIndice;
	unsigned int   tempUIntIndice;
	const int unShortByte = 2;
	const int unIntByte = 4;
	if (vComponentType == COMPONETTYPE_UNSHORT) {
		for (size_t i = vBufferView.byteOffset; i < vBufferView.byteOffset + vBufferView.byteLength; i += unShortByte) {
			std::memcpy(&tempUShortIndice, &vBuffer.data.at(i), sizeof(unsigned short));
			vIndices.push_back(tempUShortIndice);
		}
	}
	else if (vComponentType == COMPONETTYPE_UNINT) {
		for (size_t i = vBufferView.byteOffset; i < vBufferView.byteOffset + vBufferView.byteLength; i += unIntByte) {
			std::memcpy(&tempUIntIndice, &vBuffer.data.at(i), sizeof(unsigned int));
			vIndices.push_back(tempUIntIndice);
		}
	}
}

void setCNodeTransform(std::shared_ptr<Elaina::CNode> vPNode, const tinygltf::Node& vModelNode) {
	if (vModelNode.translation.size() == 3) {
		glm::vec3 tempPosition = glm::vec3((float)vModelNode.translation[0], (float)vModelNode.translation[1], (float)vModelNode.translation[2]);
		vPNode->setPosition(tempPosition);
	}
	if (vModelNode.rotation.size() == 4) {
		glm::quat tempRotationQuat = glm::quat((float)vModelNode.rotation[3], (float)vModelNode.rotation[0], (float)vModelNode.rotation[1], (float)vModelNode.rotation[2]);
		glm::vec3 euler = glm::eulerAngles(tempRotationQuat);
		glm::vec3 tempRotation = glm::degrees(euler);
		vPNode->setRotation(tempRotation);
	}
	if (vModelNode.scale.size() == 3) {
		glm::vec3 tempScale = glm::vec3((float)vModelNode.scale[0], (float)vModelNode.scale[1], (float)vModelNode.scale[2]);
		vPNode->setScale(tempScale);
	}
}


void createCNode(tinygltf::Model& vGLTFModel, std::shared_ptr<Elaina::CNode> vParentNode)
{
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;
	for (auto& node : vGLTFModel.nodes) {
		if (node.mesh == -1) continue;
		const auto& mesh = vGLTFModel.meshes[node.mesh];
		std::string MeshName = mesh.name;
		spdlog::info("MeshName : {}", MeshName);
		const auto& pChildNode = std::make_shared<Elaina::CNode>();

		for (auto& primitive : mesh.primitives) {
			Vertices.clear();
			/*if (primitive.mode == MODE_POINT) {
				const tinygltf::Accessor& accessorPos = vGLTFModel.accessors[primitive.attributes.at("POSITION")];
				const tinygltf::BufferView& bufferViewPos = vGLTFModel.bufferViews[accessorPos.bufferView];
				const tinygltf::Buffer& bufferPos = vGLTFModel.buffers[bufferViewPos.buffer];
				const tinygltf::Accessor& accessorCol = vGLTFModel.accessors[primitive.attributes.at("COLOR_0")];
				const tinygltf::BufferView& bufferViewCol = vGLTFModel.bufferViews[accessorCol.bufferView];
				const tinygltf::Buffer& bufferCol = vGLTFModel.buffers[bufferViewCol.buffer];
				glm::vec3 MinPos(accessorPos.minValues[0], accessorPos.minValues[1], accessorPos.minValues[2]);
				glm::vec3 MaxPos(accessorPos.maxValues[0], accessorPos.maxValues[1], accessorPos.maxValues[2]);

				const int vec3Byte = 12;
				for (size_t i = bufferViewPos.byteOffset, k = bufferViewCol.byteOffset;
					(i < bufferViewPos.byteOffset + bufferViewPos.byteLength && k < bufferViewCol.byteOffset + bufferViewCol.byteLength);
					i += vec3Byte, k += vec3Byte) {
					createVertexBufferData(Vertices, bufferPos, (int)i);
					createVertexBufferData(Vertices, bufferCol, (int)k);
				}

				HIVE_LOG_INFO("Vertices.size : {}", Vertices.size());
				assert(Vertices.size() == vGLTFModel.accessors[primitive.attributes.at("POSITION")].count * 3 * 2);

				const auto& pVAO = std::make_shared<hiveGL::CVertexArrayBuffer>(Vertices, std::vector<int>{3, 3}, GL_POINTS);
				const auto& pModel = std::make_shared<hiveGL::CModel>(pVAO);
				pChildNode->setPosition(-(MinPos + MaxPos) / 2.0f);
				pChildNode->addModel(pModel);
			}*/
			if (primitive.mode == MODE_TRIANGLE or primitive.mode == MODE_DEFAULT) {
				Vertices.clear();
				Indices.clear();
				const tinygltf::BufferView& bufferViewInd = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.indices].bufferView];
				const tinygltf::Buffer& bufferInd = vGLTFModel.buffers[bufferViewInd.buffer];
				const int indiceComponentType = vGLTFModel.accessors[primitive.indices].componentType;

				createIndiceBufferData(Indices, bufferViewInd, bufferInd, indiceComponentType);
				spdlog::info("indice.size : {}", Indices.size());
				assert(Indices.size() == vGLTFModel.accessors[primitive.indices].count);

				const tinygltf::BufferView& bufferViewPos = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.attributes.at("POSITION")].bufferView];
				const tinygltf::Buffer& bufferPos = vGLTFModel.buffers[bufferViewPos.buffer];
				const tinygltf::BufferView& bufferViewNor = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.attributes.at("NORMAL")].bufferView];
				const tinygltf::Buffer& bufferNor = vGLTFModel.buffers[bufferViewNor.buffer];

				assert(bufferViewPos.byteLength == bufferViewNor.byteLength);

				const int vec3Byte = 12;
				for (size_t i = bufferViewPos.byteOffset, k = bufferViewNor.byteOffset;
					(i < bufferViewPos.byteOffset + bufferViewPos.byteLength && k < bufferViewNor.byteOffset + bufferViewNor.byteLength);
					i += vec3Byte, k += vec3Byte) {

					createVertexBufferData(Vertices, bufferPos, (int)i);
					createVertexBufferData(Vertices, bufferNor, (int)k);
				}
				spdlog::info("Vertices.size : {}", Vertices.size());
				assert(Vertices.size() == vGLTFModel.accessors[primitive.attributes.at("POSITION")].count * 6);
				const auto& pVAO = Elaina::CVertexArrayObject::createVAO(Vertices, Indices, std::vector<int>{3, 3});
				const auto& pModel = std::make_shared<Elaina::CMesh>(pVAO);
				pChildNode->addMesh(pModel);
				setCNodeTransform(pChildNode, node);
			}
		}
		vParentNode->addChild(pChildNode);
	}
	return;
}

std::shared_ptr<Elaina::CNode> Elaina::CModelLoader::loadGltfFile(const std::string& vFileName)
{
	tinygltf::Model GLTFModel;
	const auto& pParentNode = std::make_shared<CNode>();
	if (!loadGLTF(vFileName, GLTFModel)) return nullptr;
	createCNode(GLTFModel, pParentNode);
	return pParentNode;
}
