#pragma once

#include "ModelInstance.h"
#include "Light.h"

class PointLight :
    public Light
{
public:
    PointLight(
        float constant,
        float linear,
        float quadratic,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        std::string uniform,
        ModelInstance modelInstance
    );

    void update();

    void draw(const Camera& camera) { modelInstance.draw(camera); }

    glm::vec3 getPosition() { return modelInstance.getTranslation(); }
    float* getPositionPtr() { return modelInstance.getTranslationPtr(); }
    void setPosition(glm::vec3 position) { modelInstance.setTranslation(position); }

    float* getScalePtr() { return modelInstance.getScalePtr(); }

    std::string getName() { return modelInstance.getName(); }

    float& getConstant() { return constant; }
    void setConstant(float constant) { this->constant = constant; }

    float& getLinear() { return linear; }
    void setLinear(float linear) { this->linear = linear; }

    float& getQuadratic() { return quadratic; }
    void setQuadratic(float quadratic) { this->quadratic = quadratic; }

    std::shared_ptr<ShaderProgram> getShader() { return modelInstance.getShader(); }

private:
    float constant;
    float linear;
    float quadratic;
    ModelInstance modelInstance;

    ModelInstance& getModelInstance() { return modelInstance; }
};

