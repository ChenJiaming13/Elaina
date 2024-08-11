#include "helper.h"
#include <array>
#include <fstream>
#include <iostream>
#include "types.h"
#include "safe.h"

bool readBinFile(const std::string& vFileName, std::vector<char>& voBuffer)
{
	std::ifstream File(vFileName, std::ios::binary);
	if (!File)
	{
		std::cerr << "cannot open this file: " << vFileName << '\n';
		return false;
	}
	File.seekg(0, std::ios::end);
	const std::streamsize Size = File.tellg();
	File.seekg(0, std::ios::beg);
	voBuffer.resize(Size);
	if (!File.read(voBuffer.data(), Size))
	{
		std::cerr << "fail to read file:" << vFileName << '\n';
	}
	File.close();
	return true;
}

bool loadRsfFile(const std::string& vFileName, std::vector<SSurfel>& voSurfels)
{
	std::vector<char> Buffer;
	if (!readBinFile(vFileName, Buffer)) return false;

	std::array<std::uint32_t, 4> Header;
	std::array<float, 6> Bounds;
	std::memcpy(Header.data(), Buffer.data(), sizeof(Header));
	std::memcpy(Bounds.data(), Buffer.data() + sizeof(Header), sizeof(Bounds));

	const std::uint32_t NumSurfels = Header[0];
	std::vector<float> PosRadiusNormal(static_cast<size_t>(NumSurfels * 8));
	std::vector<std::uint8_t> Colors(static_cast<size_t>(NumSurfels * 4));
	std::memcpy(PosRadiusNormal.data(), Buffer.data() + Header[1], sizeof(float) * PosRadiusNormal.size());
	std::memcpy(Colors.data(), Buffer.data() + Header[1] + sizeof(float) * PosRadiusNormal.size(), sizeof(std::uint8_t) * Colors.size());

	voSurfels.resize(NumSurfels);
	for (size_t i = 0; i < voSurfels.size(); ++i)
	{
		voSurfels[i]._Position = glm::vec3(
			PosRadiusNormal[i * 8 + 0],
			PosRadiusNormal[i * 8 + 1],
			PosRadiusNormal[i * 8 + 2]
		);
		voSurfels[i]._Radius = PosRadiusNormal[i * 8 + 3];
		voSurfels[i]._Normal = glm::vec3(
			PosRadiusNormal[i * 8 + 4],
			PosRadiusNormal[i * 8 + 5],
			PosRadiusNormal[i * 8 + 6]
		);
		voSurfels[i]._Color = glm::vec4(
			static_cast<float>(Colors[i * 4 + 0]) / 255.0f,
			static_cast<float>(Colors[i * 4 + 1]) / 255.0f,
			static_cast<float>(Colors[i * 4 + 2]) / 255.0f,
			static_cast<float>(Colors[i * 4 + 3]) / 255.0f
		);
	}
	return true;
}

GLuint createVAO(const std::vector<SSurfel>& vSurfels)
{
	constexpr float QuadVertices[] = {
		-0.5, -0.5,
		-0.5, 0.5,
		0.5, -0.5,
		0.5, 0.5
	};
	GLuint VAO, QuadVBO, SurfelsVBO;
	GL_SAFE_CALL(glGenVertexArrays(1, &VAO));
	GL_SAFE_CALL(glGenBuffers(1, &QuadVBO));
	GL_SAFE_CALL(glGenBuffers(1, &SurfelsVBO));
	GL_SAFE_CALL(glBindVertexArray(VAO));

	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, QuadVBO));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW));
	GL_SAFE_CALL(glEnableVertexAttribArray(0));
	GL_SAFE_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr)));

	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, SurfelsVBO));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(SSurfel) * vSurfels.size(), vSurfels.data(), GL_STATIC_DRAW));
	GL_SAFE_CALL(glEnableVertexAttribArray(1));
	GL_SAFE_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SSurfel), (void*)offsetof(SSurfel, _Position)));
	GL_SAFE_CALL(glVertexAttribDivisor(1, 1));
	GL_SAFE_CALL(glEnableVertexAttribArray(2));
	GL_SAFE_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SSurfel), (void*)offsetof(SSurfel, _Normal)));
	GL_SAFE_CALL(glVertexAttribDivisor(2, 1));
	GL_SAFE_CALL(glEnableVertexAttribArray(3));
	GL_SAFE_CALL(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SSurfel), (void*)offsetof(SSurfel, _Radius)));
	GL_SAFE_CALL(glVertexAttribDivisor(3, 1));
	GL_SAFE_CALL(glEnableVertexAttribArray(4));
	GL_SAFE_CALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(SSurfel), (void*)offsetof(SSurfel, _Color)));
	GL_SAFE_CALL(glVertexAttribDivisor(4, 1));

	GL_SAFE_CALL(glBindVertexArray(0));
	return VAO;
}
