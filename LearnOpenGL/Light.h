#pragma once

#include "glm/glm.hpp"
#include "ShaderProgram.h"

class Light
{
protected:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string uniform) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		uniform(uniform)
	{};

public:
	virtual void update() = 0;

	glm::vec3& getAmbient() { return ambient; }
	float* getAmbientPtr() { return glm::value_ptr(ambient); }
	glm::vec3 setAmbient(glm::vec3 ambient) { this->ambient = ambient; }

	glm::vec3& getDiffuse() { return diffuse; }
	float* getDiffusePtr() { return glm::value_ptr(diffuse); }
	glm::vec3 setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }

	glm::vec3& getSpecular() { return specular; }
	float* getSpecularPtr() { return glm::value_ptr(specular); }
	glm::vec3 setSpecular(glm::vec3 specular) { this->specular = specular; }

	void setShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram) { this->shaderProgram = std::move(shaderProgram); }

protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::shared_ptr<ShaderProgram> shaderProgram;
	std::string uniform;

	void updateLightProperties();
};

