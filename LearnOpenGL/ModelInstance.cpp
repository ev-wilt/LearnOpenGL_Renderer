#include "ModelInstance.h"
#include "Common.h"
#include "Camera.h"

void ModelInstance::draw(const Camera& camera)
{
    objectShader->use();

    objectShader->setFloat("material.reflectance", reflectance);
    objectShader->setFloat("material.refractance", refractance);
    objectShader->setFloat("material.refractiveIndex", refractiveIndex);
    objectShader->setFloat("material.shininess", shininess);

    // View
    objectShader->setMat4("view", camera.getView());
    objectShader->setVec3("viewPos", camera.getPosition());

    // Projection
    auto projection = glm::perspective(glm::radians(camera.getFov()), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);
    objectShader->setMat4("projection", projection);

    // Model
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, translation);
    modelMatrix = glm::scale(modelMatrix, scale);
    objectShader->setMat4("model", modelMatrix);

    // Draw call
    model->draw(objectShader);
}
