/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Render.h
Purpose: Prototype of Render class which has object informations.
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur
Creation date: 9/28/2018
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <map>

class Camera;
class Shader;

class Render
{
	unsigned m_vao, m_vbo, m_ebo, m_ubo, m_tbo, normalBuffer;
	unsigned m_elementSize, m_fnormSize;
	unsigned m_texture_1, m_texture_2;
	int width, height;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	bool isUsingTexture;

public:
	Render();
	~Render();

	void Rendering(Camera* camera, Shader* shader, float aspect, GLenum mode, glm::vec3 pos, glm::vec3 color_);
	void Describe(std::vector<glm::vec3>& vertices, std::vector<unsigned>& indices, std::vector<glm::vec2>& textures,
		std::vector<glm::vec3>& vnormals, bool isTexture);
	bool loadOBJ(const char* path, glm::vec3 & middlePoint, bool isUseTexture);
	bool loadPPM(const char* path, std::vector<glm::vec3>& values_);
	void SendTextureInfo(Shader* shader);
	void DescribeTexture();

	int whichProjection; // 0 = planar, 1 = cylindrical, 2 = spherical
	int calculateByNormal; // 0 = vertex, 1 = normal
	std::vector<glm::vec2> textureUV;
	std::vector<glm::vec3> g_spec_data;
	std::vector<glm::vec3> g_diff_data;
	std::vector<glm::vec3> obj_vertices;
	std::multimap<int, glm::vec3> faceNormals;
	std::vector<glm::vec3> vertexNormals;
	std::vector<unsigned> obj_indices;

	glm::vec3 position, scale, color;
	float rotation;
};