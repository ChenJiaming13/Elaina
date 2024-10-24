#pragma once

#include <glm/glm.hpp>

namespace Elaina
{
	class CTransform
	{
	public:
		explicit CTransform(
			const glm::vec3& vPosition = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& vRotation = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& vScale = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		
		const glm::mat4& getLocalModelMatrix() const { return m_ModelMatrix; }
		glm::vec3 getLocalPosition() const { return m_Position; }
		const glm::vec3& getLocalRotation() const { return m_Rotation; }
		const glm::vec3& getLocalScale() const { return m_Scale; }
		void setLocalPosition(const glm::vec3& vPosition);
		void setLocalRotation(const glm::vec3& vRotation);
		void setLocalScale(const glm::vec3& vScale);
	
	private:
		void __updateModelMatrix();

		glm::mat4 m_ModelMatrix;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}