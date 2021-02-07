#include "PointLight.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 color)
	:m_Pos(pos),m_Color(color) ,m_Constant(1), m_Linear(0.09f),m_Quadratic(0.032f)
{
	
}

//TODO the mesh can't be modified by the light object
//TODO make a seperate light model class
void PointLight::SetMesh(Mesh* mesh)
{
	m_Mesh = mesh;
	m_Mesh->SetPosition(m_Pos);
}

void PointLight::SetFalloffParam(float constant, float linear, float quad)
{
	m_Constant = constant;
	m_Linear = linear;
	m_Quadratic = quad;
}

void PointLight::UpdatePosition(float val)
{
	m_Pos += glm::vec3(0,val,0);
	//m_Mesh->SetPosition(m_Pos);
}
