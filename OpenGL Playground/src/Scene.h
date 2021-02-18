#pragma once

#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "light/LightManager.h"
#include "Bloom.h"
#include "GUIState.h"
#include "ScreenQuad.h"
#include "GLFW/glfw3.h"
#include "vector"


class Scene 
{
private:	
	std::vector<Camera*> m_Cameras;
	bool m_FPSMode = false;
	Camera* m_ActiveCamera;
	float m_DeltaTime;
	GUIState m_GUIState;
public:
	std::vector<Model*> m_Models; //TODO : currently public for the GUI to grab models names , fix it boy
	static Scene* active_scene;
	Scene();
	void AddModel(Model* model);
	void AddCamera(Camera* camera);


	void OnGuiUpdate();
	void OnUpdate();
	void UpdateDeltatime(float deltaTime); //Supplied by the main application

	void UpdateShaderParameters(Model* model);
	void UpdateShaderParametersinShader(Model* model, Shader* shader);
	void UpdateGlobalShadersParameters();
	void UpdateShaderParametersInShader(Shader* shader); //TODO : scene need to know if the shader requires specific data
	void Render(bool ppfx);

	void ToggleFPSMode(GLFWwindow* window);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos); //Should be overwrriten by each scene
	void static mouse_callback_dispacth(GLFWwindow* window, double xpos, double ypos);
	void static mouse_callback_nav_dispacth(GLFWwindow* window, double xpos, double ypos);

};