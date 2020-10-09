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
	GLuint lightColorID = glGetUniformLocation(m_ShaderToUse->m_ID, "color");
	m_ShaderToUse->Bind();
	glUniform3fv(lightColorID, 1, &m_Color[0]);
	m_ShaderToUse->Unbind();

}

void Light::AffectShader(const Shader& shader)
{
	//TODO shader should handle the assignment
	GLuint lightPosID = glGetUniformLocation(shader.m_ID, "light.pos");
	GLuint lightColorID = glGetUniformLocation(shader.m_ID, "light.color");
	shader.Bind();
	glUniform3fv(lightPosID, 1, &m_Pos[0]);
	glUniform3fv(lightColorID, 1, &m_Color[0]);
	shader.Unbind();
}
