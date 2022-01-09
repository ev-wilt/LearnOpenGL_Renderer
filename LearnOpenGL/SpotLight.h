#pragma once
#include "Light.h"
class SpotLight :
    public Light
{
public:
    SpotLight(
        glm::vec3 position,
        glm::vec3 direction,
        float innerCutOff,
        float outerCutOff,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        std::string uniform
    );

    void update();

    glm::vec3& getPosition() { return position; }
    void setPosition(glm::vec3 position) { this->position = position; }

    glm::vec3& getDirection() { return direction; }
    void setDirection(glm::vec3 direction) { this->direction = direction; }

    float& getInnerCutOff() { return innerCutOff; }
    void setInnerCutOff(float innerCutOff) { this->innerCutOff = innerCutOff; }

    float& getOuterCutOff() { return outerCutOff; }
    void setOuterCutOff(float outerCutOff) { this->outerCutOff = outerCutOff; }

private:
    glm::vec3 position;
    glm::vec3 direction;
    float innerCutOff;
    float outerCutOff;
};

