#pragma once

#include <vector>

#include "ModelInstance.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Camera.h"
#include "Skybox.h"

class Scene
{
public:
	Scene(std::shared_ptr<ShaderProgram> lightingProgram, std::shared_ptr<ShaderProgram> lightProgram, Camera camera, Skybox skybox);

	void addModelEntity(ModelInstance entity) { modelEntities.push_back(entity); }
	void addPointLight(PointLight light);
	void addDirectionalLight(DirectionalLight light);
	void addSpotLight(SpotLight light);

	void renderScene();

	std::vector<ModelInstance>& getModelEntities() { return modelEntities; }
	Camera& getCamera() { return camera; }
	std::vector<PointLight>& getPointLights() { return pointLights; }
	std::vector<DirectionalLight>& getDirectionalLights() { return directionalLights; }
	std::vector<SpotLight>& getSpotLights() { return spotLights; }

private:
	std::vector<ModelInstance> modelEntities;
	std::vector<PointLight> pointLights;
	std::vector<DirectionalLight> directionalLights;
	std::vector<SpotLight> spotLights;
	std::shared_ptr<ShaderProgram> lightingProgram;
	std::shared_ptr<ShaderProgram> lightProgram;
	Camera camera;
	Skybox skybox;
};

