#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GLEW/include/GL/glew.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Material.h"
#include "Vertex.h"
/*A simple mesh class
It uses one vertex array to bind the buffer data and associate a vertex array object for each mesh
Thats a perofrmance issue*/

class Mesh
{
private:
	GLuint m_ID;
	glm::vec3 m_Pos;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;

	std::vector<Vertex> m_Vertices;
	bool m_Indexed;
	std::vector<unsigned int> m_Indices;
	MaterialMap* m_Material;
public:
	//TODO REMOVE SHADER FROM MESH
	Shader* m_ShaderToUse;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialMap* material);
	Mesh(std::vector<Vertex> vertices, MaterialMap* material);
	//Utilities
	void SetMat(MaterialMap* mat);
	void SetShader(Shader* shader); //TODO Mesh can't use a shader

	void SetPosition(glm::vec3 new_pos);
	void SetScale(glm::vec3 new_scale);
	void SetRotation(glm::vec3 new_rot);

	glm::mat4 ModelMat();
	void Render();
	~Mesh();
};

#endif