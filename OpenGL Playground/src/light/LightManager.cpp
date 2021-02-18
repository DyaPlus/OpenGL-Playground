#include "LightManager.h"


LightManager::LightManager()
{
	m_LightCube = new Cube("LightCube");
	//TODO : cube size and position is controller totally by the point lights position and the shader itself
	m_LightCube->EnableInstancing();
	m_LightMaterial = new Material(); //Default Constructor Should be white mat
	m_LightMaterial->SetShader(ShaderManager::LightShader); //Setting lightcube material to use global light shader
	m_LightCube->SetMaterial(m_LightMaterial);
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

PointLight* LightManager::CreatePointLight(glm::vec3 pos, glm::vec3 color)
{
	PointLight* light = new PointLight(pos, color);
	m_PointLights.push_back(light);
	m_PointLightPositions.push_back(pos);
	return light;
}

DirectionalLight* LightManager::CreateDirectionalLight(glm::vec3 direction, glm::vec3 color)
{
	DirectionalLight* light = new DirectionalLight(direction, color);
	m_DirectionalLights.push_back(light);
	return light;
}

void LightManager::AffectShader(Shader* shader)
{
	//TODO add directional light support
	//TODO Set the values as an array

	for (int i = 0; i < m_PointLights.size(); ++i)
	{
		std::string light_element = "pointLight[" + std::to_string(i) + "].";
		shader->SetVector3(light_element + "pos", m_PointLights[i]->m_Pos);
		shader->SetVector3(light_element + "color", m_PointLights[i]->m_Color);
		shader->SetFloat(light_element + "constant", m_PointLights[i]->m_Constant);
		shader->SetFloat(light_element + "linear", m_PointLights[i]->m_Linear);
		shader->SetFloat(light_element + "quadratic", m_PointLights[i]->m_Quadratic);
	}
	shader->SetInteger("numberOfActivePLights", m_PointLights.size());
	
	for (int i = 0; i < m_DirectionalLights.size(); ++i)
	{
		std::string light_element = "dirLight[" + std::to_string(i) + "].";
		shader->SetVector3(light_element + "dir", m_DirectionalLights[i]->m_Direction);
		shader->SetVector3(light_element + "color", m_DirectionalLights[i]->m_Color);
		//TODO :: currently works for only one directional light
		shader->SetMatrix4("lightSpaceMatrix", m_DirectionalLights[i]->m_LightSpaceMatrix);
	}
	//shader->SetInteger("shadowMap", 10);
	shader->SetInteger("numberOfActiveDLights", m_DirectionalLights.size());
}

void LightManager::RenderPointLights()
{
	//TODO : its currently invoked every frame (LOL Gamed)
	m_LightCube->SetInstancingData(m_PointLightPositions.data(), m_PointLightPositions.size());
	m_LightCube->Render();
}

void LightManager::DrawPointLightsWithShader(Shader* shader)
{
	//TODO : its currently invoked every frame (LOL Gamed)
	m_LightCube->SetInstancingData(m_PointLightPositions.data(), m_PointLightPositions.size());
	m_LightCube->DrawWithShader(shader);
}

void LightManager::OnGuiUpdate()
{
	static int selected = -1;
	for (int i = 0;i < m_PointLights.size();i++)
	{
		ImGui::PushID(i);
		ImGui::Begin("Entities");
		bool selection_status = i == selected; // Used to impelement diselect functionality

		if (ImGui::Selectable(("PointLight" + std::to_string(i)).c_str(), selection_status))
		{
			if (selection_status) //Deselect if already selected
			{
				selected = -1;
			}
			else
			{
				selected = i;
			}
		}
		ImGui::End();
		if (selection_status)
		{
			ImGui::Begin("Inspector");
			if (ImGui::CollapsingHeader("Light Transform"))
			{
				if (GUI_UT::InputFloat3("Position: ", &(m_PointLights[i]->m_Pos[0])));
				{
					m_PointLightPositions[i] = m_PointLights[i]->m_Pos;
					AffectShader(ShaderManager::BasicShader);
				}
			}

			ImGui::End();
		}
		ImGui::PopID();
	}
}
