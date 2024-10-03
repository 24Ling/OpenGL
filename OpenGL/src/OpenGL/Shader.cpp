#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"


Shader::Shader(const std::string& filepath)
	:m_RendererID(0), m_Filepath(filepath)
{
	ShaderProgramSources source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader()
{
	UnBind();
	glDeleteProgram(m_RendererID);
}

ShaderProgramSources Shader::ParseShader(const std::string& filepath)
{
	std::stringstream ss[2];
	std::string line;

	std::ifstream file;
	file.open(filepath);

	if (!file.is_open())
	{
		std::cout << "Failed Open Vertex Shader File." << std::endl;
		exit(0);
	}

	enum SHADERTYPE
	{
		VERTEX = 0,
		FRAGMENT = 1,
		NONE = -1
	};

	SHADERTYPE type = SHADERTYPE::NONE;
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = SHADERTYPE::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = SHADERTYPE::FRAGMENT;
		}
		else
			ss[type] << line << "\n";
	}

	file.close();

	return { ss[SHADERTYPE::VERTEX].str(), ss[SHADERTYPE::FRAGMENT].str()};

}
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// Build and compile the vertex shader
	uint32_t id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO: Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;

	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	// Link shaders to create the shader program
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);


	// Check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


void Shader::SetUniform1i(const std::string& name, unsigned int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(std::string name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning uniform '" << name << "' doesn't exist" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}
void Shader::UnBind() const
{
	glDeleteProgram(m_RendererID);
}
