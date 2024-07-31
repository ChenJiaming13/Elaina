#include "pch.h"
#include "Primitive.h"
#include "base/VertexArrayObject.h"

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createQuad()
{
	std::vector<float> Vertices{
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	std::vector<unsigned int> Indices{
		0, 1, 2, // first Triangle
		0, 2, 3, // second Triangle
	};
	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector<int>{2, 2}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createCube()
{
	std::vector<float> Vertices 
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	std::vector<unsigned int> Indices{};
	for (unsigned int i = 0; i < 36; ++i)
	{
		Indices.push_back(i);
	}

	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector<int>{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createSphere(int vSegments)
{
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;

	for (int y = 0; y <= vSegments; ++y) {
		for (int x = 0; x <= vSegments; ++x) {
			float xSegment = (float)x / (float)vSegments;
			float ySegment = (float)y / (float)vSegments;
			float xPos = std::cos(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());
			float yPos = std::cos(ySegment * glm::pi<float>());
			float zPos = std::sin(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());

			glm::vec3 Position = glm::vec3(xPos, yPos, zPos);
			glm::vec3 Normal = glm::normalize(Position);
			glm::vec2 UV = glm::vec2(xSegment, ySegment);

			Vertices.push_back(Position.x);
			Vertices.push_back(Position.y);
			Vertices.push_back(Position.z);
			Vertices.push_back(Normal.x);
			Vertices.push_back(Normal.y);
			Vertices.push_back(Normal.z);
			Vertices.push_back(UV.x);
			Vertices.push_back(UV.y);
		}
	}

	for (int i = 0; i < vSegments; ++i) {
		for (int j = 0; j < vSegments; ++j) {
			int First = (i * (vSegments + 1)) + j;
			int Second = First + vSegments + 1;

			Indices.push_back(First);
			Indices.push_back(Second);
			Indices.push_back(First + 1);

			Indices.push_back(Second);
			Indices.push_back(Second + 1);
			Indices.push_back(First + 1);
		}
	}

	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector<int>{3, 3, 2}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createTorus(float vInnerRadius, float vOuterRadius, int vNsides, int vRings)
{
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;

	for (int i = 0; i <= vRings; ++i) {
		float Theta = i * glm::two_pi<float>() / vRings;
		float SinTheta = sin(Theta);
		float CosTheta = cos(Theta);

		for (int j = 0; j <= vNsides; ++j) {
			float Phi = j * glm::two_pi<float>() / vNsides;
			float SinPhi = sin(Phi);
			float CosPhi = cos(Phi);

			float PosX = (vOuterRadius + vInnerRadius * CosTheta) * CosPhi;
			float PosY = (vOuterRadius + vInnerRadius * CosTheta) * SinPhi;
			float PosZ = vInnerRadius * SinTheta;

			float NormalX = CosTheta * CosPhi;
			float NormalY = CosTheta * SinPhi;
			float NormalZ = SinTheta;

			// Vertex position
			Vertices.push_back(PosX);
			Vertices.push_back(PosY);
			Vertices.push_back(PosZ);
			// Vertex normal
			Vertices.push_back(NormalX);
			Vertices.push_back(NormalY);
			Vertices.push_back(NormalZ);
			// TODO uv coords
		}
	}

	for (int i = 0; i < vRings; ++i) {
		for (int j = 0; j < vNsides; ++j) {
			int First = (i * (vNsides + 1)) + j;
			int Second = First + vNsides + 1;

			Indices.push_back(First);
			Indices.push_back(Second);
			Indices.push_back(First + 1);

			Indices.push_back(Second);
			Indices.push_back(Second + 1);
			Indices.push_back(First + 1);
		}
	}

	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector<int>{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
}