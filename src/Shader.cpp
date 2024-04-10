#include "Shader.h"

#include "GL/glew.h"
#include <fstream>
#include <sstream>
#include "vendor/glm/gtc/type_ptr.hpp"

#include "ErrorCheck.h"

Shader::Shader(std::string path)
{
    ShaderSource source = parseShader(path);
    m_ID = createShader(source.vertex, source.fragment);
}

ShaderSource Shader::parseShader(const std::string& filePath)
{
    std::stringstream source[2];

    enum ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    int type = NONE;

    std::ifstream stream(filePath);

    if (!stream)
    {
        printf("Error opening file: '%s'\n", filePath.c_str());
    }

    std::string line;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = FRAGMENT;
        }
        else
        {
            source[type] << line << '\n';
        }
    }

    return { source[VERTEX].str(), source[FRAGMENT].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        printf("Failed to compile %s shader!\n", (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"));
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vSource, const std::string& fSource)
{
    unsigned int program = (glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fSource);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int Shader::getUniformLocation(std::string name)
{
    if (m_Locations.find(name) != m_Locations.end())
    {
        return m_Locations[name];
    }
    else
    {
        GLCall(int location = glGetUniformLocation(m_ID, name.c_str()));
        m_Locations[name] = location;
        return location;
    }
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_ID));
}

void Shader::useShader() const
{
    GLCall(glUseProgram(m_ID));
}

void Shader::setUniform4f(std::string name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4(std::string name, glm::mat4 mat)
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

