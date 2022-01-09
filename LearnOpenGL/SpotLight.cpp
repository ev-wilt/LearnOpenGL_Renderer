#include "SpotLight.h"

SpotLight::SpotLight(
    glm::vec3 position, 
    glm::vec3 direction, 
    float innerCutOff, 
    float outerCutOff,
    glm::vec3 ambient, 
    glm::vec3 diffuse, 
    glm::vec3 specular,
    std::string uniform
) : 
    Light(ambient, diffuse, specular, uniform),
    position(position),
    direction(direction),
    innerCutOff(innerCutOff),
    outerCutOff(outerCutOff)
{}

void SpotLight::update()
{
    updateLightProperties();
    shaderProgram->setVec3(uniform + ".position", position);
    shaderProgram->setVec3(uniform + ".direction", direction);
    shaderProgram->setFloat(uniform + ".innerCutOff", innerCutOff);
    shaderProgram->setFloat(uniform + ".outerCutOff", outerCutOff);
}
