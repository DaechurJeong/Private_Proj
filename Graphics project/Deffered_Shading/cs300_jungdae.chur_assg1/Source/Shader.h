/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: Prototype of shader class which makes shader
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur
Creation date: 9/28/2018
End Header --------------------------------------------------------*/
#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <string>

class Shader {
public:
	enum ShaderType {
		N_NONE,
		N_VERTEX,
		N_FACE,
		S_PH_SHADING,
		S_AROUND,
		S_DF_SHADING,
		G_BUFFER,
	};
	void CreateShader(const char * vertex_file_path,const char * fragment_file_path,
		const char * geometry_file_path );

	void Use() const;

	Shader(GLboolean gemoetryUse, ShaderType type);
	~Shader();

	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;
	void SetFloat(const std::string &name, float value) const;
	void SetInt(const std::string &name, int value) const;

	static std::string m_vertexShader, m_fragmentShader, m_geometryShader, 
		m_fragmentShader_two, m_vertexShader_two, m_geometryShader_two;
	GLuint m_programId, m_vertexId, m_fragmentId, m_geometryId;
	int m_infoLogLength;
	GLint m_result;
	GLboolean UseGeometry;
	ShaderType m_type;
};