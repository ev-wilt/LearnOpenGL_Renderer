#include "Scene.h"
#include "Common.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Scene::Scene(std::shared_ptr<ShaderProgram> lightingProgram, std::shared_ptr<ShaderProgram> lightProgram, Camera camera, Skybox skybox) :
    lightingProgram(std::move(lightingProgram)),
    lightProgram(std::move(lightProgram)),
    camera(camera),
    skybox(skybox)
{}

void Scene::addPointLight(PointLight light)
{
    light.setShaderProgram(lightingProgram);
    pointLights.push_back(light);
}

void Scene::addDirectionalLight(DirectionalLight light)
{
    light.setShaderProgram(lightingProgram);
    directionalLights.push_back(light);
}

void Scene::addSpotLight(SpotLight light)
{
    light.setShaderProgram(lightingProgram);
    spotLights.push_back(light);
}

void Scene::renderScene()
{
    // Update camera
    camera.update();

    // TODO: Find a better way to update uniforms in Phong shader
    lightingProgram->use();

    // Update light properties
    for (auto& pointLight : pointLights)
    {
        pointLight.update();
    }
    for (auto& dirLight : directionalLights)
    {
        dirLight.update();
    }
    for (auto& spotLight : spotLights)
    {
        spotLight.update();
    }

    // Render point lights
    for (auto& pointLight : pointLights)
    {
        pointLight.getShader()->use();
        skybox.bind(pointLight.getShader());
        pointLight.draw(camera);
    }

    // Render entites
    for (auto& entity : modelEntities)
    {
        entity.getShader()->use();
        skybox.bind(entity.getShader());
        entity.draw(camera);
    }

    // Render skybox
    skybox.draw(camera);
}
