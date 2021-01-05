#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GLEW/include/GL/glew.h"
#include <iostream>

enum class TextureType
{
	DIFFUSE , SPECULAR , NORMAL
};
class Texture2D
{
private:
	GLuint m_ID;
	TextureType m_Type;
	std::string m_Path;

public:
	Texture2D(const char* filepath , TextureType type);
	void SetWrappingMode();
	void SetFiltering();
	std::string GetPath() const;
	void Bind();
	void Unbind();
};

#endif