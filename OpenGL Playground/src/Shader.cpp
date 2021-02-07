#include "Shader.h"
	
Shader* ShaderManager::GaussianShader = nullptr;
Shader* ShaderManager::ExtractBrightnessShader = nullptr;
Shader* ShaderManager::PostProcShader = nullptr;
Shader* ShaderManager::QuadShader = nullptr;
Shader* ShaderManager::LightShader = nullptr;

void ShaderManager::Init()
{
	ExtractBrightnessShader = new Shader("res\\shaders\\basic2_bloom.glsl", ShaderType::Bloom);
	GaussianShader = new Shader("res\\shaders\\op_gaussian.glsl", ShaderType::PostProcessing);
	PostProcShader = new Shader("res\\shaders\\postprocessing.glsl", ShaderType::PostProcessing);
	QuadShader = new Shader("res\\shaders\\quadrender.glsl", ShaderType::Basic);
	LightShader = new Shader("res\\shaders\\light.glsl", ShaderType::Light);
}
ShaderSource ParseShader(const std::string& filepath) //Parse a given shader file to a ShderSource Object
{
	std::ifstream stream(filepath, std::ios::in);

	enum class ShaderSourceType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderSourceType source_type = ShaderSourceType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				source_type = ShaderSourceType::VERTEX;
			}
			if (line.find("fragment") != std::string::npos)
			{
				source_type = ShaderSourceType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)source_type] << line << "\n";
		}
	}
	return { ss[0].str() , ss[1].str() };
}

void CompileShader(GLuint id)
{
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Shader
	glCompileShader(id);

	// Check Shader Compilation Status
	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
	}
}

GLuint LoadShader(ShaderSource& Source) //Load Shader Into A Program
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Compile Vertex Shader
	printf("Compiling vertex shader\n");
	char const* VertexSourcePointer = Source.vertex_source.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	CompileShader(VertexShaderID);

	// Compile Fragment Shader
	printf("Compiling fragment shader\n");
	char const* FragmentSourcePointer = Source.fragment_source.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	CompileShader(FragmentShaderID);


	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
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

GLuint Shader::GetUniformLocation(std::string uniform_name)
{
	if (m_UniformLocationCache.find(uniform_name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[uniform_name]; //Warning : Accessing the unordered map for the first time triggers default initilization for the value of the given key
	}

	int id = glGetUniformLocation(m_ID, uniform_name.c_str());
	
	if (id == -1)
	{
		std::cout << "Warning : the uniform " << uniform_name << " couldn't be found in shader" << std::endl;
	}
	else
	{
		m_UniformLocationCache[uniform_name] = id;
	}

	return id;
}

Shader::Shader(const std::string& filepath, ShaderType type)
	:m_Type(type)
{
	m_SrcCode = ParseShader(filepath);
	m_ID = LoadShader(m_SrcCode);
}

Shader::Shader(const ShaderSource src_code, ShaderType type)
	:m_SrcCode(src_code),m_Type(type)
{
	m_ID = LoadShader(m_SrcCode);
}

ShaderType Shader::GetType()
{
	return m_Type;
}

void Shader::SetFloat(std::string uniform_name, float value)
{
	Bind();
	glUniform1f(GetUniformLocation(uniform_name), value);
	Unbind();
}

void Shader::SetInteger(std::string uniform_name, int value)
{
	Bind();
	glUniform1i(GetUniformLocation(uniform_name), value);
	Unbind();
}

void Shader::SetVector3(std::string uniform_name, glm::vec3 value)
{
	Bind();
	glUniform3fv(GetUniformLocation(uniform_name),1, &value[0]);
	Unbind();
}

void Shader::SetMatrix4(std::string uniform_name, glm::mat4 value)
{
	Bind();
	glUniformMatrix4fv(GetUniformLocation(uniform_name), 1,GL_FALSE, &value[0][0]);
	Unbind();
}

void Shader::Bind() const
{
	glUseProgram(m_ID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}
