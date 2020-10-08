#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 color, Shader* shader)
	:m_Pos(pos),m_Color(color),m_ShaderToUse(shader)
{
	
}

void Light::SetMesh(Mesh* mesh)
{
	m_Mesh = mesh;
	m_Mesh->m_ShaderToUse = m_ShaderToUse;
}
