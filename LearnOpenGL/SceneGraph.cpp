#include "SceneGraph.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

SceneGraph::SceneGraph(GLFWwindow* window, std::shared_ptr<Scene> scene) :
    window(window), scene(std::move(scene))
{
    // Initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void SceneGraph::update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("Scene Graph");

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Entities"))
        {
            for (auto& entity : scene->getModelEntities())
            {
                if (ImGui::CollapsingHeader(entity.getName().c_str()))
                {
                    ImGui::InputFloat3("Position", entity.getTranslationPtr());
                    ImGui::InputFloat3("Scale", entity.getScalePtr());
                    ImGui::SliderFloat("Reflectance", entity.getReflectancePtr(), 0.0f, 1.0f);
                    ImGui::SliderFloat("Refractance", entity.getRefractancePtr(), 0.0f, 1.0f);
                    ImGui::SliderFloat("Refractive Index", entity.getRefractiveIndexPtr(), 1.0f, 4.0f);
                    ImGui::SliderFloat("Shininess", entity.getShininessPtr(), 1.0f, 256.0f);
                }
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Lights"))
        {
            ImGui::Text("Directional Light");
            for (auto& dirLight : scene->getDirectionalLights())
            {
                ImGui::Checkbox("Enabled", dirLight.getEnabledPtr());
                ImGui::InputFloat3("Direction", dirLight.getDirectionPtr());
                ImGui::InputFloat3("Ambient", dirLight.getAmbientPtr());
                ImGui::InputFloat3("Diffuse", dirLight.getDiffusePtr());
                ImGui::InputFloat3("Specular", dirLight.getSpecularPtr());
            }

            ImGui::Text("Point Lights");
            for (auto& pointLight : scene->getPointLights())
            {
                if (ImGui::CollapsingHeader(pointLight.getName().c_str()))
                {
                    ImGui::InputFloat3("Position", pointLight.getPositionPtr());
                    ImGui::InputFloat3("Scale", pointLight.getScalePtr());
                    ImGui::InputFloat3("Ambient", pointLight.getAmbientPtr());
                    ImGui::InputFloat3("Diffuse", pointLight.getDiffusePtr());
                    ImGui::InputFloat3("Specular", pointLight.getSpecularPtr());
                }
            }

            ImGui::Text("Spot Lights");
            for (auto& spotLight : scene->getSpotLights())
            {
                ImGui::InputFloat3("Ambient", spotLight.getAmbientPtr());
                ImGui::InputFloat3("Diffuse", spotLight.getDiffusePtr());
                ImGui::InputFloat3("Specular", spotLight.getSpecularPtr());
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SceneGraph::shutDown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
