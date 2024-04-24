#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

void Camera::SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
{
	m_eye = std::move(eye);
	m_lookAt = std::move(lookat);
	m_upVector = std::move(up);
	UpdateViewMatrix();
}

void Camera::update(double dXpos, double dYpos, float elevation, float camDistance)
{
	//Fake terrain elevation
	glm::vec3 elevationVector(0.0f, elevation - m_lookAt.y, 0.0f);
	m_eye += elevationVector;
	m_lookAt += elevationVector;

	//Moving from object
	glm::vec3 direction = m_eye - m_lookAt;
	direction = glm::normalize(direction);
	m_eye = m_lookAt + direction * camDistance;

	//Updating camera position based on the mouse movement
	if (shouldMove)
	{
		glm::vec4 position(m_eye.x, m_eye.y, m_eye.z, 1.0f);
		glm::vec4 pivot(m_lookAt.x, m_lookAt.y, m_lookAt.z, 1.0f);

		float deltaAngleX = (2 * PI / WINDOW_WIDTH);
		float deltaAngleY = (PI / WINDOW_HEIGHT);

		float xAngle = dXpos * deltaAngleX;
		float yAngle = dYpos * deltaAngleY;

		glm::mat4 rotationMatrixX(1.0f);
		rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
		position = (rotationMatrixX * (position - pivot)) + pivot;

		glm::mat4 rotationMatrixY(1.0f);
		rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, getRightVector());
		glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

		SetCameraView(finalPosition, getLookAt(), m_upVector);
	}

	UpdateViewMatrix();
}