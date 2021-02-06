#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GL/glew.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Material.h"
#include "Vertex.h"
/*A simple mesh class
It uses one vertex array to bind the buffer data and associate a vertex array object for each mesh
Thats a perofrmance issue*/

class Mesh
{
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;

public:
	Material* m_Material;
	GLuint m_ID; //TODO : should be private
	std::vector<Vertex> m_Vertices; //TODO : should be private
	std::vector<unsigned int> m_Indices; //TODO : should be private
	bool m_Indexed;

	//TODO add a constructor without explicit material as argument for default materials
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material);
	Mesh(std::vector<Vertex> vertices, Material* material);
	//Utilities
	void SetMat(Material* mat);
	void SetPosition(glm::vec3 new_pos);
	void SetScale(glm::vec3 new_scale);
	void SetRotation(glm::vec3 new_rot);
	void CalculateTangents();

	glm::mat4 ModelMat();
	void Render();
	void DrawWithShader(Shader* shader);
	~Mesh();
};

#endif