#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GLEW/include/GL/glew.h"
#include <iostream>

enum class TextureType
{
	DIFFUSE , SPECULAR
};
class Texture2D
{
private:
	GLuint m_ID;
	TextureType m_Type;
public:
	Texture2D(const char* filepath , TextureType type);
	void SetWrappingMode();
	void SetFiltering();
	void Bind();
	void Unbind();
};

#endif