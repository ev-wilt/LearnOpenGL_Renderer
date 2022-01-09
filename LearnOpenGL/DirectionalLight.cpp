#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(
    glm::vec3 direction,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    std::string uniform
) :
    Light(ambient, diffuse, specular, uniform),
    direction(direction),
    enabled(true)
{};

void DirectionalLight::update()
{
    updateLightProperties();
	shaderProgram->setVec3(uniform + ".direction", direction);
    shaderProgram->setBool(uniform + ".enabled", enabled);
}
