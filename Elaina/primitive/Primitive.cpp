#include "pch.h"
#include "Primitive.h"
#include "base/VertexArrayObject.h"

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createQuad()
{
	const std::vector Vertices{
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	const std::vector<unsigned int> Indices{
		0, 1, 2, // first Triangle
		0, 2, 3, // second Triangle
	};
	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector{2, 2}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createPlane()
{
	const std::vector Vertices{
		// positions          // normals
		-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f
	};

	const std::vector<unsigned int> Indices{
		0, 1, 2,
		2, 3, 0
	};
	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createCube()
{
	const std::vector Vertices 
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

	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createSkyBox()
{
	const std::vector Vertices
	{
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	return CVertexArrayObject::createVAO(Vertices, std::vector{3}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createSphere(int vSegments)
{
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;

	for (int y = 0; y <= vSegments; ++y) {
		for (int x = 0; x <= vSegments; ++x) {
			const float xSegment = static_cast<float>(x) / static_cast<float>(vSegments);
			const float ySegment = static_cast<float>(y) / static_cast<float>(vSegments);
			const float xPos = std::cos(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());
			const float yPos = std::cos(ySegment * glm::pi<float>());
			const float zPos = std::sin(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());

			auto Position = glm::vec3(xPos, yPos, zPos);
			glm::vec3 Normal = glm::normalize(Position);
			auto UV = glm::vec2(xSegment, ySegment);

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
			const int First = (i * (vSegments + 1)) + j;
			const int Second = First + vSegments + 1;

			Indices.push_back(First);
			Indices.push_back(Second);
			Indices.push_back(First + 1);

			Indices.push_back(Second);
			Indices.push_back(Second + 1);
			Indices.push_back(First + 1);
		}
	}

	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector{3, 3, 2}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createTorus(float vInnerRadius, float vOuterRadius, int vNSides, int vRings)
{
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;

	for (int i = 0; i <= vRings; ++i) {
		const float Theta = static_cast<float>(i) * glm::two_pi<float>() / static_cast<float>(vRings);
		const float SinTheta = sin(Theta);
		const float CosTheta = cos(Theta);

		for (int j = 0; j <= vNSides; ++j) {
			const float Phi = static_cast<float>(j) * glm::two_pi<float>() / static_cast<float>(vNSides);
			const float SinPhi = sin(Phi);
			const float CosPhi = cos(Phi);

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
		for (int j = 0; j < vNSides; ++j) {
			const int First = (i * (vNSides + 1)) + j;
			const int Second = First + vNSides + 1;

			Indices.push_back(First);
			Indices.push_back(Second);
			Indices.push_back(First + 1);

			Indices.push_back(Second);
			Indices.push_back(Second + 1);
			Indices.push_back(First + 1);
		}
	}

	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createCylinder(float vRadius, float vHeight, int vSegments)
{
	std::vector<float> Vertices;
	std::vector<GLuint> Indices;

	const float HalfHeight = vHeight / 2.0f;
	const float AngleStep = glm::two_pi<float>() / static_cast<float>(vSegments);

	// Top center vertex
	Vertices.push_back(0.0f);
	Vertices.push_back(HalfHeight);
	Vertices.push_back(0.0f);
	Vertices.push_back(0.0f);
	Vertices.push_back(1.0f);
	Vertices.push_back(0.0f);

	// Bottom center vertex
	Vertices.push_back(0.0f);
	Vertices.push_back(-HalfHeight);
	Vertices.push_back(0.0f);
	Vertices.push_back(0.0f);
	Vertices.push_back(-1.0f);
	Vertices.push_back(0.0f);

	// Create vertices and normals
	for (int i = 0; i <= vSegments; ++i) {
		const float Angle = static_cast<float>(i) * AngleStep;
		float x = vRadius * cos(Angle);
		float z = vRadius * sin(Angle);

		// Top circle
		Vertices.push_back(x);
		Vertices.push_back(HalfHeight);
		Vertices.push_back(z);
		Vertices.push_back(0.0f);
		Vertices.push_back(1.0f);
		Vertices.push_back(0.0f);

		// Bottom circle
		Vertices.push_back(x);
		Vertices.push_back(-HalfHeight);
		Vertices.push_back(z);
		Vertices.push_back(0.0f);
		Vertices.push_back(-1.0f);
		Vertices.push_back(0.0f);

		// Side vertices
		Vertices.push_back(x);
		Vertices.push_back(HalfHeight);
		Vertices.push_back(z);
		Vertices.push_back(x);
		Vertices.push_back(0.0f);
		Vertices.push_back(z);

		Vertices.push_back(x);
		Vertices.push_back(-HalfHeight);
		Vertices.push_back(z);
		Vertices.push_back(x);
		Vertices.push_back(0.0f);
		Vertices.push_back(z);
	}

	// Create indices for the top and bottom circles
	for (int i = 1; i <= vSegments; ++i) {
		// Top circle
		Indices.push_back(0); // Center vertex
		Indices.push_back(i * 4);
		Indices.push_back((i % vSegments + 1) * 4);

		// Bottom circle
		Indices.push_back(1); // Center vertex
		Indices.push_back(i * 4 + 1);
		Indices.push_back((i % vSegments + 1) * 4 + 1);
	}

	// Create indices for the side surface
	for (int i = 0; i < vSegments; ++i) {
		const int Current = i * 4 + 2;
		const int Next = (i + 1) % vSegments * 4 + 2;

		Indices.push_back(Current);
		Indices.push_back(Current + 1);
		Indices.push_back(Next);

		Indices.push_back(Current + 1);
		Indices.push_back(Next + 1);
		Indices.push_back(Next);
	}

	return CVertexArrayObject::createVAO(Vertices, Indices, std::vector{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
}

std::shared_ptr<Elaina::CVertexArrayObject> Elaina::CPrimitive::createCone(float vRadius, float vHeight, int vSegments)
{
	std::vector<float> vertices;
	std::vector<GLuint> indices;

	const float HalfHeight = vHeight / 2.0f;
	const float AngleStep = glm::two_pi<float>() / static_cast<float>(vSegments);

	// Bottom center vertex
	vertices.push_back(0.0f);
	vertices.push_back(-HalfHeight);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);

	// Tip vertex
	vertices.push_back(0.0f);
	vertices.push_back(HalfHeight);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	// Create vertices and normals
	for (int i = 0; i <= vSegments; ++i) {
		const float Angle = static_cast<float>(i) * AngleStep;
		float x = vRadius * cos(Angle);
		float z = vRadius * sin(Angle);

		// Bottom circle
		vertices.push_back(x);
		vertices.push_back(-HalfHeight);
		vertices.push_back(z);
		vertices.push_back(0.0f);
		vertices.push_back(-1.0f);
		vertices.push_back(0.0f);

		// Side vertices
		vertices.push_back(x);
		vertices.push_back(-HalfHeight);
		vertices.push_back(z);
		vertices.push_back(x);
		vertices.push_back(vRadius);
		vertices.push_back(z);
	}

	// Create indices for the bottom circle
	for (int i = 1; i <= vSegments; ++i) {
		indices.push_back(0); // Center vertex
		indices.push_back(i * 2);
		indices.push_back((i % vSegments + 1) * 2);
	}

	// Create indices for the side surface
	for (int i = 0; i < vSegments; ++i) {
		const int Current = i * 2 + 2;
		const int Next = (i + 1) % vSegments * 2 + 2;

		indices.push_back(1); // Tip vertex
		indices.push_back(Current + 1);
		indices.push_back(Next + 1);
	}

	return CVertexArrayObject::createVAO(vertices, indices, std::vector{3, 3}, GL_TRIANGLES, GL_STATIC_DRAW);
}
