/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.cpp
Purpose: Create shaders and link with program
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur
Creation date: 9/28/2018
End Header --------------------------------------------------------*/
#include <vector>
#include <fstream>
#include <sstream>
#include "Shader.h"

#include "glad/glad.h"

Shader::Shader(GLboolean gemoetryUse, ShaderType type)
	:m_programId(0), m_vertexId(0), m_fragmentId(0), m_geometryId(0),
	m_infoLogLength(0), m_result(GL_FALSE), UseGeometry(gemoetryUse), m_type(type)
{
	if (gemoetryUse)
	{
		if(m_type == N_VERTEX)
			CreateShader("ShaderCodes\\VertexShader_Normal.vs",
				"ShaderCodes\\FragmentShader_Normal.fs",
				"ShaderCodes\\GeometryShader_VertexNormal.gs");
		else if(m_type == N_FACE)
			CreateShader("ShaderCodes\\VertexShader_Normal.vs",
				"ShaderCodes\\FragmentShader_Normal.fs",
				"ShaderCodes\\GeometryShader_FaceNormal.gs");
	}
	else
	{
		// Default is Phong Shading
		if (m_type == S_DF_SHADING)
			CreateShader("ShaderCodes\\Deferred_shading.vs",
				"ShaderCodes\\Deferred_shading.fs", nullptr);
		else if (m_type == S_AROUND)
			CreateShader("ShaderCodes\\Phong_Shading.vs",
				"ShaderCodes\\FragmentShader_Around.fs", nullptr);
		else if (m_type == G_BUFFER)
		{
			CreateShader("ShaderCodes\\G_buffer.vs",
				"ShaderCodes\\G_buffer.fs", nullptr);
		}
	}
}
Shader::~Shader() { glDeleteProgram(m_programId); }

void Shader::CreateShader(const char * vertex_file_path,const char * fragment_file_path,
		const char * geometry_file_path)
{
	// vertex shader
	m_vertexId = glCreateShader(GL_VERTEX_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);

	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s.\n", vertex_file_path);
		getchar();
		return;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);

	if (FragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s.\n", fragment_file_path);
		getchar();
		return;
	}

	// vertex shader
	char const* vertexSourcePointer = VertexShaderCode.c_str();
	int InfoLogLength;
	printf("Compiling shader : %s\n", vertex_file_path);
	glShaderSource(m_vertexId, 1, &vertexSourcePointer, NULL);

	glCompileShader(m_vertexId);

	glGetShaderiv(m_vertexId, GL_COMPILE_STATUS, &m_result);
	glGetShaderiv(m_vertexId, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_vertexId, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// geometry shader
	if (UseGeometry)
	{
		std::string GeometryShaderCode;
		std::ifstream GeometryShaderStream(geometry_file_path, std::ios::in);

		if (GeometryShaderStream.is_open())
		{
			std::stringstream sstr;
			sstr << GeometryShaderStream.rdbuf();
			GeometryShaderCode = sstr.str();
			GeometryShaderStream.close();
		}

		m_geometryId = glCreateShader(GL_GEOMETRY_SHADER);

		char const* geometrySourcePointer = GeometryShaderCode.c_str();

		printf("Compiling shader : %s\n", geometry_file_path);
		glShaderSource(m_geometryId, 1, &geometrySourcePointer, NULL);

		glCompileShader(m_geometryId);

		glGetShaderiv(m_geometryId, GL_COMPILE_STATUS, &m_result);
		glGetShaderiv(m_geometryId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(m_geometryId, InfoLogLength, nullptr, &GeometryShaderErrorMessage[0]);
			printf("%s\n", &GeometryShaderErrorMessage[0]);
		}
	}
	// fragment shader
	m_fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	char const* fragmentSourcePointer = FragmentShaderCode.c_str();

	printf("Compiling shader : %s\n", fragment_file_path);
	glShaderSource(m_fragmentId, 1, &fragmentSourcePointer, NULL);

	glCompileShader(m_fragmentId);

	glGetShaderiv(m_fragmentId, GL_COMPILE_STATUS, &m_result);
	glGetShaderiv(m_fragmentId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_fragmentId, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// link to program
	m_programId = glCreateProgram();
	printf("Linking program\n");
	if (m_programId == 0)
	{
		printf("Shader cannot get program id from the other.\n");
	}
	else
	{
		glAttachShader(m_programId, m_vertexId);
		if (UseGeometry)
			glAttachShader(m_programId, m_geometryId);
		glAttachShader(m_programId, m_fragmentId);

		glLinkProgram(m_programId);

		glGetProgramiv(m_programId, GL_LINK_STATUS, &m_result);
		glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(m_programId, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}
	}
	glDeleteShader(m_vertexId);
	if (UseGeometry)
		glDeleteShader(m_geometryId);
	glDeleteShader(m_fragmentId);
}

void Shader::Use() const
{
	glUseProgram(m_programId);
}
void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, false, &mat[0][0]);
}
void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}
void Shader::SetFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}
void Shader::SetInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}