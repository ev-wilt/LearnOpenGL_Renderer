#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SceneGraph.h"
#include "Model.h"
#include "ModelInstance.h"
#include "Scene.h"
#include "PostProcessingShader.h"
#include "Skybox.h"

/* Begin Globals */

int screenWidth = 1600;
int screenHeight = 1200;

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

bool firstMouseCall = true;
float lastMouseX = 400;
float lastMouseY = 300;

float yaw = -90.0f;
float pitch = 0.0f;

bool flyingMode = false;

auto camera = Camera(
    glm::vec3(0.0f, 0.0f, 3.0f),    // position
    glm::vec3(0.0f, 0.0f, -1.0f),   // local front
    glm::vec3(0.0f, 1.0f, 0.0f),    // local up
    45.0f   // fov
);

/* End Globals */

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    // Set viewport dimensions
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
}

void processInput(GLFWwindow *window, Camera &camera)
{
    float currentFrameTime = (float)glfwGetTime();
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (flyingMode)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.setPosition(camera.getPosition() + cameraSpeed * camera.getLocalFront());
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.setPosition(camera.getPosition() - cameraSpeed * camera.getLocalFront());
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.setPosition(camera.getPosition() - glm::normalize(glm::cross(camera.getLocalFront(), camera.getLocalUp())) * cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.setPosition(camera.getPosition() + glm::normalize(glm::cross(camera.getLocalFront(), camera.getLocalUp())) * cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.setPosition(camera.getPosition() + cameraSpeed * camera.getLocalUp());
    }

    static bool mouseAlreadyClicked = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !mouseAlreadyClicked)
    {
        mouseAlreadyClicked = true;
        flyingMode = !flyingMode;
        if (flyingMode)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        mouseAlreadyClicked = false;
    }
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    auto scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));

    // Set last position to current if first mouse check
    if (firstMouseCall || !flyingMode)
    {
        lastMouseX = (float)xPos;
        lastMouseY = (float)yPos;
        firstMouseCall = false;
    }

    if (flyingMode)
    {
        // Calculate change in mouse position
        float xOffset = (float)xPos - lastMouseX;
        float yOffset = (float)yPos - lastMouseY;

        lastMouseX = (float)xPos;
        lastMouseY = (float)yPos;

        // Scale by sensitivity
        const float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += -yOffset;

        // Clamp pitch value
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        scene->getCamera().updateDirection(yaw, pitch);
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));

    if (flyingMode)
        scene->getCamera().updateFov((float)yoffset);
}

int main()
{
    // Initial GLFW setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialize window
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "3D Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Enable various OpenGL features
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize shader programs
    auto phongShader = std::make_shared<ShaderProgram>("phong.vert", "phong.frag");
    auto lightShader = std::make_shared <ShaderProgram>("lights.vert", "lights.frag");

    // Render as wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Assign texture uniforms
    phongShader->use();

    auto skybox = Skybox(std::vector<std::string>{
        "./assets/skybox/right.jpg",
        "./assets/skybox/left.jpg",
        "./assets/skybox/top.jpg",
        "./assets/skybox/bottom.jpg",
        "./assets/skybox/front.jpg",
        "./assets/skybox/back.jpg"
    });

    auto scene = std::make_shared<Scene>(phongShader, lightShader, camera, skybox);
    glfwSetWindowUserPointer(window, static_cast<void*>(scene.get()));

    auto backpack = ModelInstance("Backpack", std::make_shared<Model>("./assets/models/backpack/backpack.obj"), phongShader);
    scene->addModelEntity(backpack);

    auto grassModel = std::make_shared<Model>("./assets/models/grass/grass.obj");
    grassModel->enableCulling(false);
    auto grass = ModelInstance("Grass", grassModel, phongShader, glm::vec3(3.0f, -2.0f, 0.0f));
    scene->addModelEntity(grass);


    auto sphere = std::make_shared<Model>("./assets/models/sphere/sphere.obj");

    // Lights
    scene->addDirectionalLight(
        DirectionalLight(
            glm::vec3(-0.2f, -1.0f, -0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            "dirLight"
        )
    );

    scene->addPointLight(
            PointLight(
            1.0f,
            0.09f,
            0.032f,
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            "pointLights[0]",
            ModelInstance(
                "Light A",
                sphere,
                lightShader,
                glm::vec3(0.7f, 0.2f, 2.0f),
                glm::vec3(0.2f, 0.2f, 0.2f)
            )
        )
    );

    scene->addPointLight(
        PointLight(
            1.0f,
            0.09f,
            0.032f,
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            "pointLights[1]",
            ModelInstance(
                "Light B",
                sphere,
                lightShader,
                glm::vec3(2.3f, -3.3f, -4.0f),
                glm::vec3(0.2f, 0.2f, 0.2f)
            )
        )
    );

    scene->addPointLight(
        PointLight(
            1.0f,
            0.09f,
            0.032f,
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            "pointLights[2]",
            ModelInstance(
                "Light C",
                sphere,
                lightShader,
                glm::vec3(-4.0f, 2.0f, -12.0f),
                glm::vec3(0.2f, 0.2f, 0.2f)
            )
        )
    );

    scene->addPointLight(
        PointLight(
            1.0f,
            0.09f,
            0.032f,
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            "pointLights[3]",
            ModelInstance(
                "Light D",
                sphere,
                lightShader,
                glm::vec3(0.0f, 0.0f, -3.0f),
                glm::vec3(0.2f, 0.2f, 0.2f)
            )
        )
    );

    // Initialize scene graph
    auto sceneGraph = SceneGraph(window, scene);

    // Initialize postprocessing effect
    auto effect = PostProcessingShader("postProcessing.frag");

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Read inputs
        processInput(window, scene->getCamera());

        // Update camera position
        camera.update();

        /* Begin Rendering */
        
        effect.beginRendering();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Render scene
        scene->renderScene();

        // Draw results of effect onto the screen
        effect.draw();

        // Draw scene graph
        sceneGraph.update();

        /* End Rendering */

        // Swap buffers, process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Teardown
    sceneGraph.shutDown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}