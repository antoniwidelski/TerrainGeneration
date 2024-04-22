#include "Camera.h"

void Camera::SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
{
	m_eye = std::move(eye);
	m_lookAt = std::move(lookat);
	m_upVector = std::move(up);
	UpdateViewMatrix();
}

void Camera::update(double dXpos, double dYpos, float elevation, float camDistance)
{
	m_lookAt = std::move(glm::vec3(0.0f, -elevation, 0.0f));
	glm::vec3 eyeTemp = m_eye;
	m_eye *= camDistance;
	UpdateViewMatrix();
	m_eye = eyeTemp;

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
}