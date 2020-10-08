#ifndef LIGHT_H
#define LIGHT_H


#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Mesh.h"
#include "Shader.h"

class Light
{
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
	Mesh* m_Mesh;
public:
	Shader* m_ShaderToUse;
	Light(glm::vec3 pos, glm::vec3 color , Shader* shader);
	void SetMesh(Mesh* mesh);

};
#endif 

