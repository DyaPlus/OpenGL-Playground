#define GLEW_STATIC
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "GLEW/include/GL/glew.h"
#include "GLFW/include/GLFW/glfw3.h"
#include <iostream>
#include <string>
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "CubeVertices.h"
#include "Mesh.h"
#include "light/LightManager.h"

#include "Material.h"

int width = 1024;
int height = 768;
// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);

Camera cam(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    cam.Update(xpos, ypos);
}

void UpdatePosition(Mesh &mesh)
{  
    
    glm::mat4 Model = mesh.ModelMat();
    glm::mat4 View = cam.ViewMat();
    glm::mat4 MVP = Projection * View * Model;

    mesh.m_ShaderToUse->SetMatrix4("MVP", MVP);
    if (mesh.m_ShaderToUse->GetType() == ShaderType::Basic)
    {       
        mesh.m_ShaderToUse->SetMatrix4("Model", Model);
        mesh.m_ShaderToUse->SetVector3("camPos", cam.m_CamPos);
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

    Shader BasicShader("res\\basic.glsl",ShaderType::Basic);
    Shader BasicDirShader("res\\basic_dir.glsl", ShaderType::Basic);
    Shader LightShader("res\\light.glsl", ShaderType::Light);
    Texture2D Ceramic("res\\Tiles_035_basecolor.jpg",TextureType::DIFFUSE);
    Texture2D CeramicSpec("res\\Tiles_035_roughness.jpg", TextureType::SPECULAR);

    MaterialMap CeramicMat(glm::vec3(1.0), &Ceramic, &CeramicSpec,32.0f);

    Mesh cube(vertices, sizeof(vertices) / sizeof(vertices[0]), glm::vec3(0, 0, 0), &BasicShader);
    cube.SetMat(&CeramicMat);

    Mesh light_cube(vertices, sizeof(vertices) / sizeof(vertices[0]), glm::vec3(2.0f, 0.0f, 0.0f), &BasicShader);

    PointLight * light1 = LightManager::Get()->CreatePointLight(glm::vec3(1.0f, 8.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), &LightShader);
    DirectionalLight* light2 = LightManager::Get()->CreateDirectionalLight(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    light1->SetMesh(&light_cube);
    LightManager::Get()->AffectShader(BasicShader);

    UpdatePosition(cube);
    UpdatePosition(light_cube);

    float cameraSpeed = 2.5f; // adjust accordingly
    float deltatime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    float fps;
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
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            light1->UpdatePosition(-1.2 * deltatime);
            light1->AffectShader(BasicShader);
        }
        

        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
		// Draw 
        UpdatePosition(cube);
        cube.Render();
        UpdatePosition(light_cube);
        light_cube.Render();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
		
    }
    std::cout << fps << std::endl;
}