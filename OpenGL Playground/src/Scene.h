#pragma once

#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "GLFW/glfw3.h"
#include "vector"


class Scene 
{
private:	
	std::vector<Camera*> m_Cameras;
	std::vector<Shader*> m_Shaders;
	bool m_FPSMode = false;
	Camera* m_ActiveCamera;
	float m_DeltaTime;
public:
	std::vector<Model*> m_Models; //TODO : currently public for the GUI to grab models names , fix it boy
	static Scene* active_scene;
	void AddModel(Model* model);
	void AddCamera(Camera* camera);
	void AddShader(Shader* shader);


	void OnUpdate();
	void UpdateDeltatime(float deltaTime); //Supplied by the main application

	void UpdateShaderParameters(Model* model);
	void Render();

	void ToggleFPSMode(GLFWwindow* window);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos); //Should be overwrriten by each scene
	void static mouse_callback_dispacth(GLFWwindow* window, double xpos, double ypos);
	void static mouse_callback_nav_dispacth(GLFWwindow* window, double xpos, double ypos);
};