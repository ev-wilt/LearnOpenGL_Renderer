#pragma once

#include "glm/glm.hpp"

#include "Model.h"
#include "Camera.h"
#include "Skybox.h"

class ModelInstance
{
public:
	ModelInstance(
		std::string name,
		std::shared_ptr<Model> model,
		std::shared_ptr<ShaderProgram> objectShader,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	) : 
		model(std::move(model)), 
		name(name), objectShader(objectShader),
		translation(translation), 
		scale(scale),
		reflectance(0.0f),
		refractance(0.0f),
		refractiveIndex(1.52f),
		shininess(32.0f)
	{}

	void draw(const Camera& camera);

	glm::vec3 getTranslation() { return translation; }
	void setTranslation(glm::vec3 translation) { this->translation = translation; }
	float* getTranslationPtr() { return glm::value_ptr(translation); }

	void setScale(glm::vec3 scale) { this->scale = scale; }
	float* getScalePtr() { return glm::value_ptr(scale); }

	float* getReflectancePtr() { return &reflectance; }

	float* getRefractancePtr() { return &refractance; }

	float* getRefractiveIndexPtr() { return &refractiveIndex; }

	float* getShininessPtr() { return &shininess; }

	std::string getName() { return name; }

	std::shared_ptr<ShaderProgram> getShader() { return objectShader; }

private:
	std::shared_ptr<Model> model;
	glm::vec3 translation;
	glm::vec3 scale;
	float reflectance;
	float refractance;
	float refractiveIndex;
	float shininess;
	std::string name;
	std::shared_ptr<ShaderProgram> objectShader;
};

