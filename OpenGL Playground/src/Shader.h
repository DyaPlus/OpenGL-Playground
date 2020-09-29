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

ShaderSource ParseShader(const std::string& filepath, ShaderSource& Source)
{
	std::ifstream stream(filepath,std::ios::in);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	uint8_t vertex_mod = 0;
	uint8_t fragment_mod = 0;
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
				vertex_mod = 1;
			}
			if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
				fragment_mod = 1;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	return { vertex_mod ? ss[0].str() : Source.vertex_source , fragment_mod ? ss[1].str() : Source.fragment_source };
}
GLuint LoadShader(const char* file_path, ShaderSource& Source) {

	//Parse Shader files
	Source = ParseShader(file_path, Source);
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	
	// Compile Vertex Shader
	printf("Compiling shader : %s\n", file_path);
	char const* VertexSourcePointer = Source.vertex_source.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
		
		
	// Compile Fragment Shader
	printf("Compiling shader : %s\n", file_path);
	char const* FragmentSourcePointer = Source.fragment_source.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
		

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}