#ifndef  SKYBOX_H
#define SKYBOX_H

#include "CubeMap.h"
#include "Shader.h"

class Skybox
{
private:
	GLuint m_ID;
	CubeMap* m_CubeMap;
public:
	Shader* m_ShaderToUse; //TODO : can't directly manipulate shader
	Skybox(CubeMap* cube_map, Shader* shader);
	void Render();
};

#endif // ! SKYBOX_H
