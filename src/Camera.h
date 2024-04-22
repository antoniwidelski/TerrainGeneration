#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "CommonValues.h"

class Camera
{
public:
	Camera() {}
	Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector)
		: m_eye(std::move(eye)), m_lookAt(std::move(lookAt)), m_upVector(std::move(upVector))
	{ UpdateViewMatrix(); }
	
	glm::mat4 getView() const { return m_View; }
	glm::vec3 getEye() const { return m_eye; }
	glm::vec3 getUpVector() const { return m_upVector; }
	glm::vec3 getLookAt() const { return m_lookAt; }
	glm::vec3 getViewDir() const { return -glm::transpose(m_View)[2]; }
	glm::vec3 getRightVector() const { return glm::transpose(m_View)[0]; }

	void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up);

	void UpdateViewMatrix() { m_View = glm::lookAt(m_eye, m_lookAt, m_upVector); }

	void update(double dXpos, double dYpos, float elevation, float camDistance);

	void setShouldMove(bool state) { shouldMove = state; }

private:
	glm::mat4 m_View;
	glm::vec3 m_eye;
	glm::vec3 m_lookAt;
	glm::vec3 m_upVector;

	bool shouldMove = false;
};
