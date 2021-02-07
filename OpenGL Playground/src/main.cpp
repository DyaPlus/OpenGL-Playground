#define GLEW_STATIC
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <filesystem>
#include <string>
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Mesh.h"
#include "Model.h"
#include "Shapes.h"
#include "Scene.h"
#include "light/LightManager.h"

//IMGUI
#include "GUI.h"


#include "Material.h"

int width = 1024;
int height = 768;
static GLFWwindow* active_window;
// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
//GLuint quadVAO = 0 ;
//GLuint quadVBO = 0;

Camera cam(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 3.5f);

//void RenderLightPOV(DirectionalLight* light,Model model, Shader* shader)
//{
//    glActiveTexture(GL_TEXTURE10);
//    glBindTexture(GL_TEXTURE_2D, light->m_DepthMapTex);
//    for (int i = 0; i < model.GetNumMeshes(); ++i)
//    {
//        glm::mat4 Model = model.GetMesh(i).ModelMat();
//
//        glm::mat4 MVP = light->m_LightSpaceMatrix * Model;
//        shader->SetMatrix4("MVP", MVP);
//        shader->Bind();
//        //TODO : indices and vertices shouldnt be accessed in main
//        glBindVertexArray(model.GetMesh(i).m_ID);
//        if (model.GetMesh(i).m_Indexed)
//        {
//            glDrawElements(GL_TRIANGLES, model.GetMesh(i).m_Indices.size(), GL_UNSIGNED_INT, 0);
//        }
//        else
//        {
//            glDrawArrays(GL_TRIANGLES, 0, model.GetMesh(i).m_Vertices.size());
//        }
//        glBindVertexArray(0);
//        shader->Unbind();
//        glActiveTexture(GL_TEXTURE0); //Reset default texture unit
//    }
//}
//
//void drawShadowMap(DirectionalLight* light,Shader* shadow_shader)
//{
//    shadow_shader->SetInteger("depthMap", 10);
//    shadow_shader->Bind();
//    if (quadVAO == 0)
//    {
//        float quadVertices[] = {
//            // positions        // texture Coords
//            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
//            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
//             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//        };
//
//        glGenVertexArrays(1, &quadVAO);
//        glGenBuffers(1, &quadVBO);
//        glBindVertexArray(quadVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//
//    }
//    glBindVertexArray(quadVAO);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glBindVertexArray(0);
//    shadow_shader->Unbind();
//
//}
//
////TODO : Should be added to scene as a model / object , when shader abstraction is added
//void UpdatePosition(Skybox& skybox)
//{
//    
//    glm::mat4 View = glm::mat4(glm::mat3(cam.ViewMat()));
//    glm::mat4 MVP = Projection * View;
//
//    skybox.m_ShaderToUse->SetMatrix4("MVP", MVP);
//
//}
//
//void UpdatePositionEnvMap(Model& model,CubeMap& cube_map)
//{
//    for (int i = 0; i < model.GetNumMeshes(); ++i)
//    {
//        glm::mat4 Model = model.GetMesh(i).ModelMat();
//        glm::mat4 View = cam.ViewMat();
//        glm::mat4 MVP = Projection * View * Model;
//
//        model.GetMesh(i).m_ShaderToUse->SetMatrix4("MVP", MVP);
//        //Envmap Updtae
//        model.GetMesh(i).m_ShaderToUse->SetInteger("skybox", 11);
//        glActiveTexture(GL_TEXTURE11); // We set the skybox to be 11
//        //Envmap Update
//        cube_map.Bind(); // We Bind the cubemap to txture 11
//        if (model.GetMesh(i).m_ShaderToUse->GetType() == ShaderType::Basic)
//        {
//            model.GetMesh(i).m_ShaderToUse->SetMatrix4("Model", Model);
//            model.GetMesh(i).m_ShaderToUse->SetVector3("camPos", cam.m_CamPos);
//        }
//    }
//
//}

void InitGLFW()
{
    //GLFW INIT
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
}

void InitGLEW()
{
    //GLEW INIT
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }
    //glViewport(0, 0, width, height);
}

int main(void)
{
    // GLFW Init
    InitGLFW();
    GLFWwindow* window; 
    window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    active_window = window;
    //GLEW Init
    InitGLEW();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Scene scene1;
    Scene::active_scene = &scene1;
    //TODO : Active Scene should be handled by a window class not passing the window itself
    Scene::active_scene->ToggleFPSMode(window); 

    //Create Shaders
    // Shader BasicShader("res\\shaders\\basic.glsl",ShaderType::Basic); //Basic Phong Shader
    Shader BasicBlinnShader("res\\shaders\\basic2.glsl", ShaderType::Basic); //Basic Blinn-Phong Shader
    Shader LightShader("res\\shaders\\light.glsl", ShaderType::Light);
    //Shader SkyboxShader("res\\shaders\\skybox_shader.glsl", ShaderType::Basic); //Box uses it to render skybox
    //Shader EnvMapShader("res\\shaders\\environment_map.glsl", ShaderType::Basic); //Model Uses it for EnvMap 
    //Shader LightPovShader("res\\shaders\\light_pov.glsl", ShaderType::Light);
    //Shader ShadowShader("res\\shaders\\shadow.glsl", ShaderType::Light);

    //Create Texture
    Texture2D Ceramic("res\\Tiles_035_basecolor.jpg",TextureType::DIFFUSE);
    Texture2D CeramicSpec("res\\Tiles_035_roughness.jpg", TextureType::SPECULAR);

    //Create Materials
    Material* WhiteMat = new Material();

    //Setup Lights
    PointLight * light1 = LightManager::Get()->CreatePointLight(glm::vec3(0, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    //DirectionalLight* light2 = LightManager::Get()->CreateDirectionalLight(glm::vec3(0, 0, -2.0f), glm::vec3(3, 3, 3));

    //TODO : find a better lighting binding operation
    LightManager::Get()->AffectShader(&BasicBlinnShader);
    LightManager::Get()->AffectShader(ShaderManager::ExtractBrightnessShader);

    Cube cubetestmodel("BrickCube");
    cubetestmodel.SetRotation(glm::vec3(90, 0, 0));
    cubetestmodel.SetPosition(glm::vec3(0, 3, -3.5f));
    cubetestmodel.SetScale(glm::vec3(10.0f, 1, 10));

    Model naruto("res/models/naruto/D0401253.obj","Naruto");
    naruto.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    naruto.SetPosition(glm::vec3(0, 1, 0));

    //Cube whitecube("LightCube");
    //whitecube.SetMaterial(WhiteMat);
    //Util
    float deltatime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float fps;
    bool srgb_enable = true;
    bool srgb_pressed = false;

    //Configure SRGB
    if (!srgb_enable)
    {
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
    else if (srgb_enable)
    {
        glEnable(GL_FRAMEBUFFER_SRGB); //Inform opengl that the incoming fragment values are in linear space , so force conversion
    }

    //Configure Scene
    scene1.AddCamera(&cam);
    scene1.AddShader(&BasicBlinnShader);
    scene1.AddModel(&naruto);
    //scene1.AddModel(&whitecube);
    scene1.AddModel(&cubetestmodel);

    glEnable(GL_DEPTH_TEST);
    bool* selected = new bool;
    *selected = false;
    while (!glfwWindowShouldClose(window))
    {
        
        //// 1. List Models
        //{
        //    static int counter = 0;

        //    ImGui::Begin("Models");
        //    
        //    for (auto model : Scene::active_scene->m_Models)
        //    {

        //        if (ImGui::Selectable(model->GetName().c_str(), selected))
        //        {
        //            *selected = true;
        //        }
        //    }

        //    if (ImGui::Button("Add Light"))
        //    {
        //        LightManager::Get()->CreatePointLight(cam.m_CamPos, glm::vec3(10.0f, 10.0f, 10.0f), &LightShader);
        //        LightManager::Get()->AffectShader(&BasicBlinnShader);
        //    }
        //    ImGui::SameLine();
        //    ImGui::Text("Models = %d", counter);

        //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //    ImGui::End();
        //}


        float currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps = 1 / deltatime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.Forward(deltatime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.Backward(deltatime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.Left(deltatime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.Right(deltatime);
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            Scene::active_scene->ToggleFPSMode(window);
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            //light1->UpdatePosition(1.2 * deltatime);

        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            //light1->UpdatePosition(-1.2 * deltatime);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !srgb_pressed)
        {
            srgb_pressed = true;
            if (!srgb_enable)
            {
                glEnable(GL_FRAMEBUFFER_SRGB);
                srgb_enable = true;
                std::cout << "enabled SRGB" << std::endl;
            }
            else if (srgb_enable)
            {
                glDisable(GL_FRAMEBUFFER_SRGB);
                srgb_enable = false;
                std::cout << "disabled SRGB" << std::endl;

            }
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        {
            srgb_pressed = false;
        }
        
        //Update Depth Maps , TODO : Should be controlled by scene object
        //TODO : only works for one directional light
        /* light2->SetDepthMap();
        RenderLightPOV(light2,testmodel, &LightPovShader);
        RenderLightPOV(light2,cubetestmodel, &LightPovShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //Return to original viewport 
        glViewport(0, 0, width, height);*/
        

		// Draw 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Scene::active_scene->UpdateDeltatime(deltatime);
        Scene::active_scene->Render(false);

        //drawShadowMap(light2, &ShadowShader); //To draw the shadowDepth map
        GUI::GUI_Instance->StartFrame();

        GUI::GUI_Instance->EndFrame();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << fps << std::endl;
}