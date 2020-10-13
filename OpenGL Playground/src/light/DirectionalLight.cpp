#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color)
	:m_Direction(direction),m_Color(color)
{
	
}

void DirectionalLight::AffectShader(const Shader& shader)
{
	//TODO shader should handle the assignment
	GLuint lightDirID = glGetUniformLocation(shader.m_ID, "light.direction");
	GLuint lightColorID = glGetUniformLocation(shader.m_ID, "light.color");
	shader.Bind();
	glUniform3fv(lightDirID, 1, &m_Direction[0]);
	glUniform3fv(lightColorID, 1, &m_Color[0]);
	shader.Unbind();
}
