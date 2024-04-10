#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "vendor/glm/gtc/matrix_transform.hpp"

struct ShaderSource
{
	std::string vertex;
	std::string fragment;
};

class Shader
{

private:
	unsigned int m_ID;

	std::unordered_map<std::string, int> m_Locations;

private:
	ShaderSource parseShader(const std::string& filePath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vSource, const std::string& fSource);

	int getUniformLocation(std::string name);

public:
	Shader() {}
	Shader(std::string path);
	~Shader();

	void useShader() const;
	void setUniform4f(std::string name, float v0, float v1, float v2, float v3);
	void setUniformMat4(std::string name, glm::mat4 mat);
};