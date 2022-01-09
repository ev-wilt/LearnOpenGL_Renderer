#pragma once

#include "Cubemap.h"
#include "ShaderProgram.h"
#include "Camera.h"

#include <string>
#include <vector>

class Skybox :
    public Cubemap
{
public:
    Skybox(std::vector<std::string> faces);

    void draw(const Camera& camera);
    void setupCubeMesh();
    void bind(std::shared_ptr<ShaderProgram> program) const;

private:
    static const float vertices[];
    ShaderProgram skyboxProgram;
    unsigned int cubeVAO;
    const static GLenum texId;
    const static int uniformId;
};

