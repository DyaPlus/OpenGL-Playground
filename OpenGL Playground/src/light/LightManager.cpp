#include "LightManager.h"

LightManager::LightManager()
{

}

LightManager* LightManager::m_Instance = 0; //Actual Definition of the static member
LightManager* LightManager::Get()
{
	if (!m_Instance)
	{
		m_Instance = new LightManager();
	}
	return m_Instance;
}

PointLight* LightManager::CreatePointLight(glm::vec3 pos, glm::vec3 color, Shader* shader)
{
	PointLight* light = new PointLight(pos, color, shader);
	m_PointLights.push_back(light);
	return light;
}

DirectionalLight* LightManager::CreateDirectionalLight(glm::vec3 direction, glm::vec3 color)
{
	DirectionalLight* light = new DirectionalLight(direction, color);
	m_DirectionalLights.push_back(light);
	return light;
}

void LightManager::AffectShader(Shader& shader)
{
	//TODO add directional light support
	//TODO Set the values as an array

	for (int i = 0; i < m_PointLights.size(); ++i)
	{
		std::string light_element = "light[" + std::to_string(i) + "].";
		shader.SetVector3(light_element + "pos", m_PointLights[i]->m_Pos);
		shader.SetVector3(light_element + "color", m_PointLights[i]->m_Color);
		shader.SetFloat(light_element + "constant", m_PointLights[i]->m_Constant);
		shader.SetFloat(light_element + "linear", m_PointLights[i]->m_Linear);
		shader.SetFloat(light_element + "quadratic", m_PointLights[i]->m_Quadratic);
	}

	shader.SetInteger("numberOfActiveLights", m_PointLights.size());
	
}
