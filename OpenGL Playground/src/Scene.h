#pragma once

#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "GLFW/glfw3.h"
#include "vector"


class Scene 
{
private:	
	std::vector<Model*> m_Models;
	std::vector<Camera*> m_Cameras;
	std::vector<Shader*> m_Shaders;
	Camera* m_ActiveCamera;
	float m_DeltaTime;
public:
	static Scene* active_scene;
	void AddModel(Model* model);
	void AddCamera(Camera* camera);
	void AddShader(Shader* shader);


	void OnUpdate();
	void UpdateDeltatime(float deltaTime); //Supplied by the main application

	void UpdateShaderParameters(Model* model);
	void Render();

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void static mouse_callback_dispacth(GLFWwindow* window, double xpos, double ypos);
};