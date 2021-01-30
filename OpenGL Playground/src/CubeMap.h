#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "GL/glew.h"
#include <vector>
#include <iostream>

class CubeMap
{
private:
	GLuint m_ID;
	std::string m_Directory;

public:
	CubeMap(std::string directory, std::vector<std::string> faces);
	void Bind();
	void Unbind();

};

#endif