#pragma once

#include <memory>

namespace Elaina
{
	class CVertexArrayObject;
	class CPrimitive
	{
	public:
		static std::shared_ptr<CVertexArrayObject> createQuad();
		static std::shared_ptr<CVertexArrayObject> createPlane();
		static std::shared_ptr<CVertexArrayObject> createCube();
		static std::shared_ptr<CVertexArrayObject> createSkyBox();
		static std::shared_ptr<CVertexArrayObject> createSphere(int vSegments = 64);
		static std::shared_ptr<CVertexArrayObject> createTorus(float vInnerRadius = 0.2f, float vOuterRadius = 0.5f, int vNSides = 30, int vRings = 30);
		static std::shared_ptr<CVertexArrayObject> createCylinder(float vRadius = 0.5f, float vHeight = 1.0f, int vSegments = 32);
		static std::shared_ptr<CVertexArrayObject> createCone(float vRadius = 0.5f, float vHeight = 1.0f, int vSegments = 32);
	};
}