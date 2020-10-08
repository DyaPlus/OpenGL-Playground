#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GLEW/include/GL/glew.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Shader.h"

/*A simple mesh class
It uses one vertex array to bind the buffer data and associate a vertex array object for each mesh
Thats a perofrmance issue*/

class Mesh
{
private:
	float* m_Vertices;
	GLuint m_ID;
	glm::vec3 m_Pos;
public:
	Shader* m_ShaderToUse;
	Mesh(float* vertices,int length,glm::vec3 pos,Shader* shader);
	glm::mat4 ModelMat();
	void Render();
	~Mesh();
};

#endif