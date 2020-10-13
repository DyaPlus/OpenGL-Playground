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

void LightManager::AffectShader(const Shader& shader)
{
	shader.Bind();
	//TODO add directional light support
	for (int i = 0; i < m_PointLights.size(); ++i)
	{
		std::string light_element = "light[" + std::to_string(i) + "].";
		//TODO shader should handle the assignment
		GLuint lightPosID = glGetUniformLocation(shader.m_ID, (light_element + "pos").c_str());
		GLuint lightColorID = glGetUniformLocation(shader.m_ID, (light_element + "color").c_str());
		GLuint lightFalloffConstID = glGetUniformLocation(shader.m_ID, (light_element + "constant").c_str());
		GLuint lightFalloffLinearID = glGetUniformLocation(shader.m_ID, (light_element + "linear").c_str());
		GLuint lightFalloffQuadID = glGetUniformLocation(shader.m_ID, (light_element + "quadratic").c_str());

		glUniform3fv(lightPosID, 1, &(m_PointLights[i]->m_Pos[0]));
		glUniform3fv(lightColorID, 1, &(m_PointLights[i]->m_Color[0]));
		glUniform1f(lightFalloffConstID, m_PointLights[i]->m_Constant);
		glUniform1f(lightFalloffLinearID, m_PointLights[i]->m_Linear);
		glUniform1f(lightFalloffQuadID, m_PointLights[i]->m_Quadratic);
	}

	GLuint numOfLightsID = glGetUniformLocation(shader.m_ID, "numberOfActiveLights");
	glUniform1i(numOfLightsID, m_PointLights.size());
	shader.Unbind();
}
