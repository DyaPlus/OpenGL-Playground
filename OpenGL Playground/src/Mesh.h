#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GL/glew.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Material.h"
#include "Vertex.h"

#define MAX_INSTANCES 50
/*A simple mesh class
It uses one vertex array to bind the buffer data and associate a vertex array object for each mesh
Thats a perofrmance issue*/

class Mesh
{
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;
	GLuint m_InstanceVBO;

	bool m_Instantiable;
	unsigned int m_NumInstances;
public:
	Material* m_Material;
	GLuint m_ID; //TODO : should be private

	std::vector<Vertex> m_Vertices; //TODO : should be private
	std::vector<unsigned int> m_Indices; //TODO : should be private
	bool m_Indexed;

	//TODO add a constructor without explicit material as argument for default materials
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material);
	Mesh(std::vector<Vertex> vertices, Material* material, unsigned int num_of_instances = 0); //Setting num_of_instances in initialization sets the instancing on

	//Utilities
	void EnableInstancing();

	void SetMat(Material* mat);
	void SetPosition(glm::vec3 new_pos);
	void SetScale(glm::vec3 new_scale);
	void SetRotation(glm::vec3 new_rot);
	void SetInstancingData(glm::vec3* data,unsigned int num_of_instances);
	void CalculateTangents();

	glm::mat4 ModelMat();
	void Render();
	void DrawWithShader(Shader* shader);
	~Mesh();
};

#endif