#include "PointLight.h"

PointLight::PointLight(
    float constant,
    float linear,
    float quadratic,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    std::string uniform,
    ModelInstance modelInstance
) :
    Light(ambient, diffuse, specular, uniform),
    constant(constant),
    linear(linear),
    quadratic(quadratic),
    modelInstance(modelInstance)
{};

void PointLight::update()
{
    updateLightProperties();
    shaderProgram->setVec3(uniform + ".position", modelInstance.getTranslation());
    shaderProgram->setFloat(uniform + ".constant", constant);
    shaderProgram->setFloat(uniform + ".linear", linear);
    shaderProgram->setFloat(uniform + ".quadratic", quadratic);
}
