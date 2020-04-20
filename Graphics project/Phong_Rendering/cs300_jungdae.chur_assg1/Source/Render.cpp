/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Render.cpp
Purpose: Contain informations of object and draw it.
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur , ??
<provide your name, student login, and student id>
Creation date: 9/28/2018
End Header --------------------------------------------------------*/

#define PI 3.141592654
#define TWOPI 6.283185308
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>

#include "Render.h"
#include "Shader.h"
#include "Camera.h"
#include <string>
#include <iostream>

Render::Render()
	: position(glm::vec3()), scale(glm::vec3(1, 1, 1)), color(glm::vec3(1.0f, 1.0f, 1.0f)), rotation(0.f),
	xMax(0), xMin(0), yMax(0), yMin(0), isUsingTexture(false), whichProjection(1), calculateByNormal(false), width(512), height(512)
{
}
Render::~Render()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_tbo);
}
void Render::Describe(std::vector<glm::vec3> vertices, std::vector<unsigned> indices, std::vector<glm::vec2> textures)
{
	glGenVertexArrays(1, &m_vao);

	glGenBuffers(1, &m_vbo);

	glGenBuffers(1, &m_ebo);

	glGenBuffers(1, &m_tbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	if (isUsingTexture)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_tbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	}

	const GLsizei vertex_size_stride = 0;
	constexpr GLint three_components_in_vertex_position = 3;
	constexpr GLint two_components_in_vertex_normal = 3;
	constexpr GLint three_components_in_vertex_texture_coordinates = 2;
	constexpr GLenum float_element_type = GL_FLOAT;
	constexpr GLboolean not_fixedpoint = GL_FALSE;
	const void* position_offset_in_vertex = reinterpret_cast<void*>(0);
	const void* normal_offset_in_vertex = reinterpret_cast<void*>(0);
	const void* texture_coordinates_offset_in_vertex = reinterpret_cast<void*>(0);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, three_components_in_vertex_position, float_element_type, not_fixedpoint, vertex_size_stride, position_offset_in_vertex);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, two_components_in_vertex_normal, float_element_type, not_fixedpoint, vertex_size_stride, normal_offset_in_vertex);
	if (isUsingTexture)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, three_components_in_vertex_texture_coordinates, float_element_type, not_fixedpoint, vertex_size_stride, texture_coordinates_offset_in_vertex);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_elementSize = (unsigned)indices.size() * sizeof(unsigned);

	
}
void Render::Rendering(Camera* camera, Shader* shader, float aspect, GLenum mode, glm::vec3 pos, glm::vec3 color_)
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);

	glm::mat4 model = glm::translate(identity_translate, pos) * glm::scale(identity_scale, scale) * glm::rotate(identity_rotation, rotation, up);
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();

	if (isUsingTexture)
	{
		shader->SetInt("projtype", whichProjection);
		shader->SetInt("calculateByNormal", calculateByNormal);
	}
	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);
	shader->SetVec3("color", color_);
	if (shader->UseGeometry)
		shader->SetFloat("time", static_cast<float>(glfwGetTime()));

	glBindVertexArray(m_vao);
	if(isUsingTexture)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(textureUV.size() * sizeof(glm::vec2)), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
	else
	{
		glDrawElements(mode, m_elementSize, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}
bool Render::loadOBJ(const char * path, glm::vec3 & middlePoint, bool isUseTexture)
{
	float max_x = 0, min_x = 0, max_y = 0, min_y = 0, max_z = 0, min_z = 0, abs_max = 0;
	isUsingTexture = isUseTexture;
	FILE * file = fopen(path, "r");
	if (file == NULL) 
	{
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			// Save min & max
			xMax = max_x;
			xMin = min_x;
			yMax = max_y;
			yMin = min_y;
			/* Set position and scale to default [-1, 1] */
			if (abs_max < glm::abs(max_x - min_x))
				abs_max = glm::abs(max_x - min_x);
			if (abs_max < glm::abs(max_y - min_y))
				abs_max = glm::abs(max_y - min_y);
			if (abs_max < glm::abs(max_z - min_z))
				abs_max = glm::abs(max_z - min_z);

			middlePoint = glm::vec3((max_x + min_x) / (2 * abs_max),
				(max_y + min_y) / (2 * abs_max),
				(max_z + min_z) / (2 * abs_max));

			xMax /= (0.5f * abs_max);
			xMin /= (0.5f * abs_max);
			yMax /= (0.5f * abs_max);
			yMin /= (0.5f * abs_max);
			zMax /= (0.5f * abs_max);
			zMin /= (0.5f * abs_max);
			xMax -= (2.f * middlePoint.x);
			xMin -= (2.f * middlePoint.x);
			yMax -= (2.f * middlePoint.y);
			yMin -= (2.f * middlePoint.y);
			zMax -= (2.f * middlePoint.z);
			zMin -= (2.f * middlePoint.z);
			for (unsigned i = 0; i < obj_vertices.size(); ++i)
			{
				obj_vertices[i].x /= (0.5f * abs_max);
				obj_vertices[i].y /= (0.5f * abs_max);
				obj_vertices[i].z /= (0.5f * abs_max);
				obj_vertices[i] -= (2.f * middlePoint);
				if (isUseTexture)
				{
					glm::vec3 normalized = glm::normalize(obj_vertices[i]);

					float theta = glm::atan(normalized.y / normalized.x);
					glm::vec2 textUV = glm::vec2(theta / TWOPI, (normalized.z + 1) * 0.5f);

					textureUV.push_back(textUV);
				}
			}
			break;
		}
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			/* Find the min & max data which is in obj */
			if (max_x > vertex.x)
				max_x = vertex.x;
			if (max_y > vertex.y)
				max_y = vertex.y;
			if (max_z > vertex.z)
				max_z = vertex.z;
			
			if (min_x < vertex.x)
				min_x = vertex.x;
			if (min_y < vertex.y)
				min_y = vertex.y;
			if (min_z < vertex.z)
				min_z = vertex.z;

			obj_vertices.push_back(vertex);
		}

		if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3];
			int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
			if (matches != 3)
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			obj_indices.push_back(vertexIndex[0] - 1);
			obj_indices.push_back(vertexIndex[1] - 1);
			obj_indices.push_back(vertexIndex[2] - 1);
		}
	}
	return true;
}
bool Render::loadPPM(const char* path, std::vector<glm::vec3>& values_)
{
	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Impossible to open the file !\n");
		return false;
	}
	char fileHeader[128] = { 0 };
	int res = fscanf(file, "%s\n", &fileHeader);
	res = fscanf(file, "%d %d\n", &width, &height);
	while (1)
	{
		int x = 0, y = 0, z = 0;
		int end = fscanf(file, "%d\n%d\n%d", &x, &y, &z);
		if (end == EOF)
			break;

		glm::vec3 as_vec(static_cast<float>(x/255.f), static_cast<float>(y/255.f), static_cast<float>(z/255.f));

		values_.push_back(as_vec);
	}
	return true;
}
void Render::SendTextureInfo(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_1);
	GLint texSamplerLoc_1 = glGetUniformLocation(shader->m_programId, "ourTexture1");
	glUniform1i(texSamplerLoc_1, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture_2);
	GLint texSamplerLoc_2 = glGetUniformLocation(shader->m_programId, "ourTexture2");
	glUniform1i(texSamplerLoc_2, 1);
}

void Render::DescribeTexture()
{
	// Now generate the textures and bind it
	glGenTextures(1, &m_texture_1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
		(const GLfloat *)g_diff_data.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &m_texture_2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture_2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
		(const GLfloat *)g_spec_data.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	return;
}