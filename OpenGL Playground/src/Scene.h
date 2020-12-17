#pragma once

#include "Model.h"
#include "Camera.h"
#include "Shader.h"

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
	void AddModel(Model* model);
	void AddCamera(Camera* camera);
	void AddShader(Camera* shader);
	void Render();
	void OnUpdate();
	void UpdateShaderParameters(Model* model);
	void UpdateDeltatime(float deltaTime);
};