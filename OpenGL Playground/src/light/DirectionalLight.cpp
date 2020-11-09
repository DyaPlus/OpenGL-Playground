#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color)
	:m_Direction(direction),m_Color(color)
{

	glm::mat4 View = glm::lookAt(glm::vec3(-7, -7.0f, -7) * direction, direction, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Projection = glm::ortho(-12.0f, 12.0f, -12.0f, 12.0f, 1.0f, 7.5f);
	m_LightSpaceMatrix = Projection * View ;

	//Create Framebuffer
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	m_DepthMapFBO = depthMapFBO;

	//Create Texture for depth map
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	m_DepthMapTex = depthMap;
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//TODO remove as the light manager is setting the values
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

void DirectionalLight::SetDepthMap()
{
	glViewport(0, 0, 1024, 1024); //TODO : make width and height configurable 
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_DepthMapTex); //Bind depthMap to texture 10
}


