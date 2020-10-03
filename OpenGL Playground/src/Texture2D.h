#include "GLEW/include/GL/glew.h"
#include <iostream>
class Texture2D
{
private:
	GLuint m_ID;
public:
	Texture2D(const char* filepath);
	void SetWrappingMode();
	void SetFiltering();
	void Bind();
	void Unbind();
};