#pragma once
#include "Light.h"

class DirectionalLight :
    public Light
{
public:
    DirectionalLight(
        glm::vec3 direction,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        std::string uniform
    );

    void update();

    glm::vec3& getDirection() { return direction; }
    float* getDirectionPtr() { return glm::value_ptr(direction); }
    void setDirection(glm::vec3 direction) { this->direction = direction; }

    bool* getEnabledPtr() { return &enabled; }

private:
    glm::vec3 direction;
    bool enabled;
};

