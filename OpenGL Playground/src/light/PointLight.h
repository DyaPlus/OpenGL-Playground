#ifndef POINTLIGHT_H
#define POINTLIGHT_H


#include "src/vendor/glm/gtc/matrix_transform.hpp"
#include "src/Mesh.h"
#include "src/Shader.h"

class PointLight
{
	//TODO : memebrs can't be public
public:
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
	Mesh* m_Mesh;
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
public:
	PointLight(glm::vec3 pos, glm::vec3 color);
	void SetMesh(Mesh* mesh);
	void SetFalloffParam(float constant,float linear,float quad);
	void UpdatePosition(float val);
};
#endif 

