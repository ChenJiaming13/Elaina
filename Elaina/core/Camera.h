#pragma once

namespace Elaina
{
	class CCamera
	{
	public:
		enum class ECameraType { PERSP, ORTHO };

	public:
		CCamera(ECameraType vCameraType = ECameraType::PERSP, float vAspect = 1.0f, float vNearHeight = 1.0f, float vNear = 0.1f, float vFar = 100.0f);
		
		const ECameraType& getCameraType() const { return m_CameraType; }
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::vec3& getWorldPos() const { return m_Position; }
		const glm::vec3& getFront() const { return m_Front; }
		const glm::vec3& getUp() const { return m_Up; }
		const glm::vec3& getRight() const { return m_Right; }
		const glm::vec3& getWorldUp() const { return m_WorldUp; }
		const glm::vec4& getSolidColor() const { return m_SolidColor; }
		float getAspect() const { return m_Aspect; }
		float getNear() const { return m_Near; }
		float getFar() const { return m_Far; }
		float getNearHeight() const { return m_NearHeight; }
		
		void setAspect(float vAspect);
		void setNear(float vNear);
		void setFar(float vFar);
		void setNearHeight(float vNearHeight);
		void setWorldPos(const glm::vec3& vPosition);
		void setFront(const glm::vec3& vFront);
		void setWorldUp(const glm::vec3& vWorldUp);
		void setViewport(int vWidth, int vHeight);
		void setSolidColor(const glm::vec4& vSolidColor) { m_SolidColor = vSolidColor; }
	
	private:
		void __updateProjectionMatrix();
		void __updateViewMatrix();
		void __updateCameraVectors();
		
		ECameraType m_CameraType;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		glm::vec4 m_SolidColor;
		float m_Aspect;
		float m_Near;
		float m_Far;
		float m_NearHeight;
	};
}