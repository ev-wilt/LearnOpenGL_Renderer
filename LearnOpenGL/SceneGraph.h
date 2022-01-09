#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Scene.h"

class SceneGraph
{
public:
	SceneGraph(GLFWwindow* window, std::shared_ptr<Scene> scene);

	void update();
	void shutDown();

private:
	GLFWwindow* window;
	std::shared_ptr<Scene> scene;
};

