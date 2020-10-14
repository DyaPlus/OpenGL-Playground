#include "PointLight.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, Shader* shader)
	:m_Pos(pos),m_Color(color),m_ShaderToUse(shader) ,m_Constant(1), m_Linear(0.09f),m_Quadratic(0.032f)
{
	
}

//TODO the mesh can't be modified by the light object
void PointLight::SetMesh(Mesh* mesh)
{
	m_Mesh = mesh;
	m_Mesh->ChangePosition(m_Pos);
	m_Mesh->m_ShaderToUse = m_ShaderToUse;
	m_ShaderToUse->SetVector3("color", m_Color);
}

void PointLight::SetFalloffParam(float constant, float linear, float quad)
{
	m_Constant = constant;
	m_Linear = linear;
	m_Quadratic = quad;
}

//TODO REMOVE AS LightManager is setting the values 
void PointLight::AffectShader(const Shader& shader)
{
	//TODO shader should handle the assignment
	GLuint lightPosID = glGetUniformLocation(shader.m_ID, "light[0].pos");
	GLuint lightColorID = glGetUniformLocation(shader.m_ID, "light[0].color");
	GLuint lightFalloffConstID = glGetUniformLocation(shader.m_ID, "light[0].constant");
	GLuint lightFalloffLinearID = glGetUniformLocation(shader.m_ID, "light[0].linear");
	GLuint lightFalloffQuadID = glGetUniformLocation(shader.m_ID, "light[0].quadratic");

	shader.Bind();
	glUniform3fv(lightPosID, 1, &m_Pos[0]);
	glUniform3fv(lightColorID, 1, &m_Color[0]);
	glUniform1f(lightFalloffConstID, m_Constant);
	glUniform1f(lightFalloffLinearID, m_Linear);
	glUniform1f(lightFalloffQuadID, m_Quadratic);
	shader.Unbind();
}

void PointLight::UpdatePosition(float val)
{
	m_Pos += glm::vec3(0,val,0);
	m_Mesh->ChangePosition(m_Pos);
}
