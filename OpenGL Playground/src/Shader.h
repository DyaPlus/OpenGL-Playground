#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "GLEW/include/GL/glew.h"

struct ShaderSource {
	std::string vertex_source;
	std::string fragment_source;
};

class Shader {
private:
	GLuint m_ID;
	ShaderSource m_SrcCode;
public:
	Shader(const std::string& filepath);
	Shader(const ShaderSource src_code);
	void Bind() const;
	void Unbind() const;
};

ShaderSource ParseShader(const std::string& filepath); //Parse a given shader file to a ShderSource Object
void CompileShader(GLuint id);
GLuint LoadShader(ShaderSource& Source); //Load Shader Into A Program


#endif