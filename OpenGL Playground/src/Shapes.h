#ifndef SHAPES_H
#define SHAPES_H

#include "Model.h"

class Cube : public Model
{
private:
	
public:
	Cube(std::string name, unsigned int num_of_instances = 0);
	void SetInstancingData(glm::vec3* data, unsigned int num_of_instances);
};

#endif