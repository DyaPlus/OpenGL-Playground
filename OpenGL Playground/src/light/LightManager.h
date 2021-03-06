#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "PointLight.h"
#include "DirectionalLight.h"
#include "src/Shapes.h"
#include "src/Material.h"
#include "string"

//TODO : give each light an ID
class LightManager
{
private:
	LightManager();
	static LightManager* m_Instance; //IT IS ONLY DECLARATION NOT DEFINITION , Any attempt to use it shall rise a linker error no definition
	std::vector<PointLight*> m_PointLights;
	std::vector<DirectionalLight*> m_DirectionalLights;
	Cube* m_LightCube;
	Material* m_LightMaterial;
	std::vector<glm::vec3> m_PointLightPositions; //used for instancing in the gpu
public:
	static LightManager* Get();
	PointLight* CreatePointLight(glm::vec3 pos, glm::vec3 color);
	DirectionalLight* CreateDirectionalLight(glm::vec3 direction, glm::vec3 color);
	void AffectShader(Shader* shader);
	void RenderPointLights();
	void DrawPointLightsWithShader(Shader* shader);
	void OnGuiUpdate();
};

#endif