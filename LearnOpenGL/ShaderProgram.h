#pragma once

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class ShaderProgram
{
public:

    ShaderProgram(const char *vertexPath, const char *fragmentPath);

    // the program ID
    unsigned int id;

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    float getFloat(const std::string& name) const;
    void setFloat(const std::string &name, float value) const;

    void setMat4(const std::string& name, glm::mat4 value) const;

    glm::vec3 getVec3(const std::string& name) const;
    void setVec3(const std::string& name, glm::vec3 value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
};

#endif