#pragma once

#include <memory>

namespace Elaina
{
	class CVertexArrayBuffer;
	class CPrimitive
	{
	public:
		static std::shared_ptr<CVertexArrayBuffer> createCube();
		static std::shared_ptr<CVertexArrayBuffer> createSphere(int vSegments = 64);
		static std::shared_ptr<CVertexArrayBuffer> createTorus(float vInnerRadius = 0.2f, float vOuterRadius = 0.5f, int vNsides = 30, int vRings = 30);
	};
}