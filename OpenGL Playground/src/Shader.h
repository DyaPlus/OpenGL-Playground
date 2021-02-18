#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "GL/glew.h"
#include "vendor/glm/gtc/matrix_transform.hpp"

struct ShaderSource {
	std::string vertex_source;
	std::string fragment_source;
};

enum class ShaderType
{
	Basic , Light , Bloom , PostProcessing
};

class Shader {
private:
	ShaderSource m_SrcCode;
	GLuint GetUniformLocation(std::string uniform_name);
	std::unordered_map<std::string , GLuint> m_UniformLocationCache;
	ShaderType m_Type;
public:
	Shader() = default;
	GLuint m_ID;
	Shader(const std::string& filepath , ShaderType type);
	Shader(const ShaderSource src_code, ShaderType type);
	ShaderType GetType();
	void SetFloat(std::string uniform_name, float value);
	void SetInteger(std::string uniform_name, int value);
	void SetVector3(std::string uniform_name, glm::vec3 value);
	void SetMatrix4(std::string uniform_name, glm::mat4 value);

	void Bind() const;
	void Unbind() const;
};

struct ShaderManager
{
	static void Init();
	static Shader* BasicShader;
	static Shader* ExtractBrightnessShader;
	static Shader* GaussianShader;
	static Shader* PostProcShader;
	static Shader* QuadShader;
	static Shader* LightShader;

};

ShaderSource ParseShader(const std::string& filepath); //Parse a given shader file to a ShderSource Object
void CompileShader(GLuint id);
GLuint LoadShader(ShaderSource& Source); //Load Shader Into A Program

#endif