#include "Light.h"

void Light::updateLightProperties()
{
	shaderProgram->setVec3(uniform + ".ambient", ambient);
	shaderProgram->setVec3(uniform + ".diffuse", diffuse);
	shaderProgram->setVec3(uniform + ".specular", specular);
}
