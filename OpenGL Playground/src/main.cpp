#define GLEW_STATIC
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "GLEW/include/GL/glew.h"
#include "GLFW/include/GLFW/glfw3.h"
#include <iostream>
#include <filesystem>
#include <string>
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Mesh.h"
#include "Model.h"
#include "CubeMap.h"
#include "Skybox.h"

#include "light/LightManager.h"

#include "Material.h"

int width = 1024;
int height = 768;
// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);

Camera cam(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 3.5f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    cam.Update(xpos, ypos);
}

void UpdatePosition(Model &model)
{  
    for (int i = 0; i < model.GetNumMeshes(); ++i)
    {
        glm::mat4 Model = model.GetMesh(i).ModelMat();
        glm::mat4 View = cam.ViewMat();
        glm::mat4 MVP = Projection * View * Model;

        model.GetMesh(i).m_ShaderToUse->SetMatrix4("MVP", MVP);
        if (model.GetMesh(i).m_ShaderToUse->GetType() == ShaderType::Basic)
        {       
            model.GetMesh(i).m_ShaderToUse->SetMatrix4("Model", Model);
            model.GetMesh(i).m_ShaderToUse->SetVector3("camPos", cam.m_CamPos);
        }
    }

}
void UpdatePosition(Skybox& skybox)
{
    
    glm::mat4 View = glm::mat4(glm::mat3(cam.ViewMat()));
    glm::mat4 MVP = Projection * View;

    skybox.m_ShaderToUse->SetMatrix4("MVP", MVP);

}

void UpdatePositionEnvMap(Model& model,CubeMap& cube_map)
{
    for (int i = 0; i < model.GetNumMeshes(); ++i)
    {
        glm::mat4 Model = model.GetMesh(i).ModelMat();
        glm::mat4 View = cam.ViewMat();
        glm::mat4 MVP = Projection * View * Model;

        model.GetMesh(i).m_ShaderToUse->SetMatrix4("MVP", MVP);
        //Envmap Updtae
        model.GetMesh(i).m_ShaderToUse->SetInteger("skybox", 11);
        glActiveTexture(GL_TEXTURE11); // We set the skybox to be 11
        //Envmap Update
        cube_map.Bind(); // We Bind the cubemap to txture 11
        if (model.GetMesh(i).m_ShaderToUse->GetType() == ShaderType::Basic)
        {
            model.GetMesh(i).m_ShaderToUse->SetMatrix4("Model", Model);
            model.GetMesh(i).m_ShaderToUse->SetVector3("camPos", cam.m_CamPos);
        }
    }

}
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
}

int main(void)
{
    InitGLFW();
    // Open a window and create its OpenGL context
    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    InitGLEW();
    //TODO associate with a scene object
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    //Create Shaders
    Shader BasicShader("res\\shaders\\basic.glsl",ShaderType::Basic); //Basic Phong Shader
    Shader BasicBlinnShader("res\\shaders\\basic2.glsl", ShaderType::Basic); //Basic Blinn-Phong Shader

    Shader BasicDirShader("res\\shaders\\basic_dir.glsl", ShaderType::Basic); 
    Shader LightShader("res\\shaders\\light.glsl", ShaderType::Light);
    Shader SkyboxShader("res\\shaders\\skybox_shader.glsl", ShaderType::Basic); //Box uses it to render skybox
    Shader EnvMapShader("res\\shaders\\environment_map.glsl", ShaderType::Basic); //Model Uses it for EnvMap 

    //Create Texture
    Texture2D Ceramic("res\\Tiles_035_basecolor.jpg",TextureType::DIFFUSE);
    Texture2D CeramicSpec("res\\Tiles_035_roughness.jpg", TextureType::SPECULAR);

    //Create Materials
    MaterialMap CeramicMat( &Ceramic, &CeramicSpec,32.0f);

    //Setup Lights
    PointLight * light1 = LightManager::Get()->CreatePointLight(glm::vec3(1.0f, 8.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), &LightShader);

    DirectionalLight* light2 = LightManager::Get()->CreateDirectionalLight(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    LightManager::Get()->AffectShader(BasicShader);
    LightManager::Get()->AffectShader(BasicBlinnShader);

    //Create Model
    //Model testmodel("res/models/backpack/backpack.obj");
    Model testmodel("res/models/modern_chair/modern chair 11 obj.obj");
    testmodel.SetScale(glm::vec3(.05f, 0.05, 0.05));
    testmodel.SetPosition(glm::vec3(0, 2, 0));

    testmodel.SetShader(&BasicShader);

    Model cubetestmodel;
    cubetestmodel.SetScale(glm::vec3(10.0f, 1, 10));
    cubetestmodel.SetShader(&BasicBlinnShader);

    //Create Cubemap
    std::vector<std::string> faces = 
    {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    CubeMap cube_map("res/skybox/skybox1/",faces);

    //Create Skybox
    Skybox skybox(&cube_map, &SkyboxShader); //Skybox Class taking the cubemap and the shader to render

    float deltatime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    float fps;
    bool srgb_enable = true;
    bool srgb_pressed = false;

    if (!srgb_enable)
    {
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
    else if (srgb_enable)
    {
        glEnable(GL_FRAMEBUFFER_SRGB);
        

    }

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
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
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            light1->UpdatePosition(1.2 * deltatime);
            light1->AffectShader(BasicShader);
            light1->AffectShader(BasicBlinnShader);

        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            light1->UpdatePosition(-1.2 * deltatime);
            light1->AffectShader(BasicShader);
            light1->AffectShader(BasicBlinnShader);
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
        

        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
		// Draw 
        UpdatePosition(skybox);
        skybox.Render();

        UpdatePosition(testmodel);
        testmodel.Render();

        UpdatePosition(cubetestmodel);
        cubetestmodel.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
		
    }
    std::cout << fps << std::endl;
}