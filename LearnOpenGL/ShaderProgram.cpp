#include "ShaderProgram.h"

// Creates a shader file given its source code and the shader type
unsigned int createShader(const char *code, GLenum shaderType)
{
    auto id = glCreateShader(shaderType);
    glShaderSource(id, 1, &code, NULL);
    glCompileShader(id);

    // Error handling
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    return id;
}

// Parses the source code from a single shader file
std::string parseShaderFile(const char *path)
{
    std::string code;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream shaderStream;
        shaderFile.open(path);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    return code;
}

ShaderProgram::ShaderProgram(const char *vertexPath, const char *fragmentPath) : 
    id(glCreateProgram())
{
    auto vertexCode = parseShaderFile(vertexPath);
    auto vertexId = createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    glAttachShader(id, vertexId);

    auto fragmentCode = parseShaderFile(fragmentPath);
    auto fragmentId = createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    glAttachShader(id, fragmentId);

    glLinkProgram(id);

    // Error handling
    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Shader cleanup
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void ShaderProgram::use()
{
    glUseProgram(id);
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

float ShaderProgram::getFloat(const std::string& name) const
{
    float value;
    glGetUniformfv(id, glGetUniformLocation(id, name.c_str()), &value);
    return value;
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
}

glm::vec3 ShaderProgram::getVec3(const std::string& name) const
{
    float values[3];
    glGetnUniformfv(id, glGetUniformLocation(id, name.c_str()), 3, values);
    return glm::vec3(values[0], values[1], values[2]);
}
