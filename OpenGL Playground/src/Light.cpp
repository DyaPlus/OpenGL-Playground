#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 color, Shader* shader)
	:m_Pos(pos),m_Color(color),m_ShaderToUse(shader)
{
	
}

//TODO the mesh can't be modified by the light object
void Light::SetMesh(Mesh* mesh)
{
	m_Mesh = mesh;
	m_Mesh->ChangePosition(m_Pos);
	m_Mesh->m_ShaderToUse = m_ShaderToUse;
}

void Light::AffectShader(const Shader& shader)
{
	//TODO shader should handle the assignment
	GLuint lightID = glGetUniformLocation(shader.m_ID, "lightPos");
	shader.Bind();
	glUniform3fv(lightID, 1, &m_Pos[0]);
	shader.Unbind();
}
