#ifndef POINTLIGHT_H
#define POINTLIGHT_H


#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Mesh.h"
#include "Shader.h"

class PointLight
{
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
	Mesh* m_Mesh;
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
public:
	Shader* m_ShaderToUse;
	PointLight(glm::vec3 pos, glm::vec3 color , Shader* shader);
	void SetMesh(Mesh* mesh);
	void SetFalloffParam(float constant,float linear,float quad);
	void AffectShader(const Shader& shader);
	void UpdatePosition(float val);
};
#endif 

