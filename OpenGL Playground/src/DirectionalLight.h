#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H


#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Shader.h"

class DirectionalLight
{
private:
	glm::vec3 m_Direction;
	glm::vec3 m_Color;
public:
	DirectionalLight(glm::vec3 direction, glm::vec3 color);
	void AffectShader(const Shader& shader);
};

#endif 

