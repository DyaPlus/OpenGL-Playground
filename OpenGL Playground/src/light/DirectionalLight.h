#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H


#include "src/vendor/glm/gtc/matrix_transform.hpp"
#include "src/Shader.h"

class DirectionalLight
{
public:
	glm::vec3 m_Direction;
	glm::vec3 m_Color;
	GLuint m_DepthMapFBO;
	GLuint m_DepthMapTex;
	glm::mat4 m_LightSpaceMatrix;
public:
	DirectionalLight(glm::vec3 direction, glm::vec3 color);
	void AffectShader(const Shader& shader);
	void SetDepthMap();
	
};

#endif 

