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
#define MAX_DEPTH 10

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
	m_sphere.m_center = glm::vec3(0, 0, 0);
	m_sphere.m_radius = 1.f;
}
Render::~Render()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_tbo);
	glDeleteBuffers(1, &normalBuffer);

	glDeleteVertexArrays(1, &m_debug_vao);
	glDeleteBuffers(1, &m_debug_vbo);
	glDeleteBuffers(1, &m_debug_ebo);
	glDeleteBuffers(1, &m_debug_color);
}
void AddObject(Render* obj_list, int index, glm::vec3* mid_point,
	Render* obj_list_debug, Render* obj_bounding_box, Render* obj_aabb, ShapeType shape_,
	std::vector<Render*>& aabb_obj_vec, std::vector<Render*>& real_obj_vec, std::vector<glm::vec3>& all_point,
	std::vector<std::vector<glm::vec3>>& points_in_triangle,
	std::vector<std::vector<unsigned>>& indices_in_triangle,
	std::vector<int>& triangle_obj_number)
{
	// Set all object settings
	float x_ = (float)(rand() % 8) - 4.f;
	float y_ = (float)(rand() % 8);
	float z_ = (float)(rand() % 8) - 4.f;
	obj_list[index].position = glm::vec3(x_, y_, z_);
	obj_list[index].scale = glm::vec3(1, 1, 1);

	int rand_obj_list = rand() % 9;
	const char* path = "";
	switch (rand_obj_list)
	{
	case 0:
		path = "models\\bunny_high_poly.obj";
		break;
	case 1:
		path = "models\\cube_high_poly.obj";
		break;
	case 2:
		path = "models\\cube_low_poly.obj";
		break;
	case 3:
		path = "models\\horse_high_poly.obj";
		break;
	case 4:
		path = "models\\menger_sponge_level_1_high_poly.obj";
		break;
	case 5:
		path = "models\\menger_sponge_level_1_low_poly.obj";
		break;
	case 6:
		path = "models\\sphere_high_poly.obj";
		break;
	case 7:
		path = "models\\sphere_mid_poly.obj";
		break;
	case 8:
		path = "models\\teapot_mid_poly.obj";
		break;
	default:
		break;
	}
		
	obj_list[index].loadOBJ(path, mid_point, index, false);
	if(!obj_list[index].loadPPM("models\\metal_roof_diff_512x512.ppm", obj_list[index].g_diff_data))
		std::cout << "No PPM diffuse file " << index << " opened" << std::endl;
	if(!obj_list[index].loadPPM("models\\metal_roof_spec_512x512.ppm", obj_list[index].g_spec_data))
		std::cout << "No PPM specular file " << index << " opened" << std::endl;
	
	// Set for octree
	obj_list_debug[index].obj_vertices = obj_list[index].obj_vertices;
	obj_list_debug[index].obj_indices = obj_list[index].obj_indices;

	for (unsigned i = 0; i < obj_list[index].obj_vertices.size(); ++i)
	{
		glm::vec3 world_pos_point = obj_list[index].obj_vertices[i] + obj_list[index].position;
		all_point.push_back(world_pos_point);
	}
	// Set for bounding box
	obj_bounding_box[index].position = obj_list[index].position;
	obj_bounding_box[index].color = { 0.8f, 0.8f, 0.9f };
	obj_bounding_box[index].scale = { 1.f, 1.f, 1.f };
	obj_bounding_box[index].m_BVtype = Render::BoundingVolumeType::AABB;
	// Set for AABB
	obj_aabb[index].position = obj_list[index].position;
	obj_aabb[index].color = { 0.8f, 0.8f, 0.9f };
	obj_aabb[index].scale = { 1.f, 1.f, 1.f };
	obj_aabb[index].m_BVtype = Render::BoundingVolumeType::AABB;
	if (shape_ == AABB)
	{
		if (!obj_bounding_box[index].loadOBJ("models\\cube_low_poly.obj", mid_point, index, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return;
		}
		if (!obj_aabb[index].loadOBJ("models\\cube_low_poly.obj", mid_point, index, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return;
		}
	}
	else
	{
		if (!obj_bounding_box[index].loadOBJ("models\\sphere_mid_poly.obj", mid_point, index, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return;
		}
		if (!obj_aabb[index].loadOBJ("models\\sphere_mid_poly.obj", mid_point, index, false))
		{
			std::cout << "Failed to read object_center OBJ file!" << std::endl;
			return;
		}
	}

	obj_list[index].Describe(obj_list[index].obj_vertices, obj_list[index].obj_indices,
		obj_list[index].textureUV, obj_list[index].vertexNormals, false);
	obj_list[index].DescribeTexture();

	obj_bounding_box[index].Describe(obj_bounding_box[index].obj_vertices, obj_bounding_box[index].obj_indices,
		obj_bounding_box[index].textureUV, obj_bounding_box[index].vertexNormals, false);

	obj_aabb[index].Describe(obj_aabb[index].obj_vertices, obj_aabb[index].obj_indices,
		obj_aabb[index].textureUV, obj_aabb[index].vertexNormals, false);

	obj_bounding_box[index].scale = obj_list[index].CalculateBoundingBoxScale() * 0.5f;
	obj_aabb[index].scale = obj_list[index].CalculateBoundingBoxScale() * 0.5f;

	aabb_obj_vec.push_back(&obj_aabb[index]);
	real_obj_vec.push_back(&obj_list[index]);
	obj_list[index].obj_colors.resize(obj_list[index].obj_vertices.size());

	for (int i = 0; i < obj_list[index].obj_indices.size(); i += 3)
	{
		std::vector<glm::vec3> each_indice_to_point;
		std::vector<unsigned> each_indice;
		each_indice_to_point.push_back(obj_list[index].obj_vertices[obj_list[index].obj_indices[i]] + obj_list[index].position);
		each_indice_to_point.push_back(obj_list[index].obj_vertices[obj_list[index].obj_indices[i + 1]] + obj_list[index].position);
		each_indice_to_point.push_back(obj_list[index].obj_vertices[obj_list[index].obj_indices[i + 2]] + obj_list[index].position);
		each_indice.push_back(obj_list[index].obj_indices[i]);
		each_indice.push_back(obj_list[index].obj_indices[i + 1]);
		each_indice.push_back(obj_list[index].obj_indices[i + 2]);

		indices_in_triangle.push_back(each_indice);
		points_in_triangle.push_back(each_indice_to_point);
		triangle_obj_number.push_back(index);
	}
}
void TopDownBVTree(HierarchyManager** tree, std::vector<glm::vec3> point_, int depth_, ShapeType shape_)
{
	// all points should be in point_ at first.
	if (point_.size() == 0)
		return;
	const int MIN_POINTS_PER_LEAF = 500;
	HierarchyManager* pNode = new HierarchyManager;
	*tree = pNode;
	glm::vec3 max_axis = ComputeBoundingVolume(point_, pNode->BVposition, pNode->BVscale, shape_);
	pNode->depth = depth_;

	if (point_.size() <= (size_t)MIN_POINTS_PER_LEAF)
	{
		pNode->points = point_;
	}
	else
	{
		std::vector<glm::vec3> left_points;
		std::vector<glm::vec3> right_points;
		if (max_axis.y == 0 && max_axis.z == 0) // x_axis
		{
			for (int i = 0; i < point_.size(); ++i)
			{
				if (max_axis.x < point_[i].x)
					left_points.push_back(point_[i]);
				else
					right_points.push_back(point_[i]);
			}
		}
		else if (max_axis.x == 0 && max_axis.z == 0) // y_axis
		{
			for (int i = 0; i < point_.size(); ++i)
			{
				if (max_axis.y < point_[i].y)
					left_points.push_back(point_[i]);
				else
					right_points.push_back(point_[i]);
			}
		}
		else if (max_axis.x == 0 && max_axis.y == 0) // z_axis
		{
			for (int i = 0; i < point_.size(); ++i)
			{
				if (max_axis.z < point_[i].z)
					left_points.push_back(point_[i]);
				else
					right_points.push_back(point_[i]);
			}
		}
		TopDownBVTree(&(pNode->left_), left_points, depth_ + 1, shape_);
		TopDownBVTree(&(pNode->right_), right_points, depth_ + 1, shape_);
	}
}
HierarchyManager* BottomUpBVTree(std::vector<Render*> obj, ShapeType shape_)
{
	if (obj.empty())
		return nullptr;
	int i, j = 0;
	int num_obj = static_cast<int>(obj.size());

	HierarchyManager** pNodes = new HierarchyManager*[num_obj];

	for (int k = 0; k < num_obj; ++k)
	{
		pNodes[k] = new HierarchyManager;
		pNodes[k]->objects = obj[k];
		pNodes[k]->BVposition = obj[k]->position;
		pNodes[k]->BVscale = obj[k]->scale;
		pNodes[k]->depth = 1;
	}
	while (num_obj > 1) {
		FindNodesToMerge(&pNodes[0], num_obj, i, j);
		HierarchyManager* pPair = new HierarchyManager;
		pPair->left_ = pNodes[i];
		pPair->right_ = pNodes[j];

		ComputeBoundingVolume_BottomUp(pNodes[i], pNodes[j],
			pPair->BVposition, pPair->BVscale, shape_);

		pPair->depth = pNodes[i]->depth + 1;

		int min = i, max = j;
		if (i > j)
			min = j, max = i;
		pNodes[min] = pPair;
		pNodes[max] = pNodes[num_obj - 1];
		--num_obj;
	}
	HierarchyManager* pRoot = pNodes[0];
	delete pNodes;
	return pRoot;
}
BSPNode::BSPNode(std::vector<std::vector<glm::vec3>> triangle, std::vector<Render*> obj,
	std::vector<int> obj_number, std::vector<std::vector<unsigned>> indiceList)
{
	float R_ = (rand() % 255) / 255.f;
	float G_ = (rand() % 255) / 255.f;
	float B_ = (rand() % 255) / 255.f;
	glm::vec3 rand_color = glm::vec3(R_, G_, B_);
	for (int i = 0; i < triangle.size(); ++i)
	{
		int obj_index = obj_number[i];
		obj[obj_index]->obj_colors[indiceList[i][0]] = rand_color;
		obj[obj_index]->obj_colors[indiceList[i][1]] = rand_color;
		obj[obj_index]->obj_colors[indiceList[i][2]] = rand_color;
	}
	m_triangle_list = triangle;
}
Plane ComputePlane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	Plane p;
	p.normal = glm::normalize(glm::cross(b - a, c - a));
	p.dot = glm::dot(p.normal, a);
	return p;
}
BSPNode* BuildBSPTree(std::vector<Render*> obj, std::vector<std::vector<glm::vec3>>& triangleList, std::vector<int> obj_number, 
	std::vector<std::vector<unsigned>> indiceList, int depth_)
{
	BSPNode* pNode = new BSPNode;
	pNode->depth = depth_;

	// Terminate recursive loop. Leaf storing will set color of objects
	if (triangleList.size() < 300 || depth_ > MAX_DEPTH)
		return new BSPNode(triangleList, obj, obj_number, indiceList);

	Plane splitPlane = PickSplittingPlane(triangleList);
	// To reset all values for front and back child
	std::vector<std::vector<glm::vec3>> frontList, backList;
	std::vector<int> front_new_obj_number, behind_new_obj_number;
	std::vector<std::vector<unsigned>> front_new_indiceList, behind_new_indiceList;

	for (int i = 0; i < triangleList.size(); ++i)
	{
		int obj_index = obj_number[i];

		std::vector<glm::vec3> curr_tri = triangleList[i];
		std::vector<unsigned> curr_tri_indice = indiceList[i];
		int currTri_type = ClassifyTriangleToPlane(curr_tri, splitPlane);
		if (currTri_type == COPLANAR)
			continue;
		else if (currTri_type == FRONT)
		{
			frontList.push_back(curr_tri);
			front_new_obj_number.push_back(obj_index);
			front_new_indiceList.push_back(curr_tri_indice);
		}
		else if (currTri_type == BEHIND)
		{
			backList.push_back(curr_tri);
			behind_new_obj_number.push_back(obj_index);
			behind_new_indiceList.push_back(curr_tri_indice);
		}
		else if (currTri_type == STRADDLING)
		{
			frontList.push_back(curr_tri);
			backList.push_back(curr_tri);
			front_new_obj_number.push_back(obj_index);
			behind_new_obj_number.push_back(obj_index);
			front_new_indiceList.push_back(curr_tri_indice);
			behind_new_indiceList.push_back(curr_tri_indice);
		}
	}

	pNode->front = BuildBSPTree(obj, frontList, front_new_obj_number, front_new_indiceList, depth_ + 1);
	pNode->back = BuildBSPTree(obj, backList, behind_new_obj_number, behind_new_indiceList, depth_ + 1);
	return pNode;
}

Plane PickSplittingPlane(std::vector<std::vector<glm::vec3>>& tri_indice_list)
{
	// Blend factor for balance or split
	const float K = 0.8f;

	Plane bestPlane;
	float bestScore = FLT_MAX;

	for (int i = 0; i < tri_indice_list.size(); ++i)
	{
		int numFront = 0, numBehind = 0, numStraddling = 0;
		Plane plane = ComputePlane(tri_indice_list[i][0], tri_indice_list[i][1], tri_indice_list[i][2]);
		for (int j = 0; j < tri_indice_list.size(); ++j)
		{
			if (i == j)
				continue;
			int result = ClassifyTriangleToPlane(tri_indice_list[j], plane);

			if (result == COPLANAR)
				continue;
			else if (result == FRONT)
				++numFront;
			else if (result == BEHIND)
				++numBehind;
			else if (result == STRADDLING)
				++numStraddling;
		}
		float score = K * numStraddling + (1.0f - K) * std::abs(numFront - numBehind);
		if (score < bestScore)
		{
			bestScore = score;
			bestPlane = plane;
		}
	}
	return bestPlane;
}
int ClassifyTriangleToPlane(std::vector<glm::vec3> triangle_indice, Plane plane)
{
	int numFront = 0, numBehind = 0;
	for (int i = 0; i < 3; ++i) //  only 3 points in tri
	{
		glm::vec3 point_ = triangle_indice[i];
		int result = ClassifyPointToPlane(point_, plane);
		if (result == POINT_FRONT)
			++numFront;
		else if (result == POINT_BEHIND)
			++numBehind;
	}
	if (numBehind != 0 && numFront != 0)
		return STRADDLING;
	if (numFront != 0)
		return FRONT;
	if (numBehind != 0)
		return BEHIND;
	return COPLANAR;
}
int ClassifyPointToPlane(glm::vec3 point, Plane plane)
{
	float dist = glm::dot(plane.normal, point) - plane.dot;
	if(dist > std::numeric_limits<float>::epsilon())
		return POINT_FRONT;
	if(dist < -std::numeric_limits<float>::epsilon())
		return POINT_BEHIND;
	return POINT_ON;
}
glm::vec3 IntersectEdgeAgainstPlane(glm::vec3 p1, glm::vec3 p2, Plane plane)
{
	glm::vec3 dir_ = p2 - p1;
	float nDotP1 = glm::dot(plane.normal, p1);
	float nDotDir = glm::dot(plane.normal, dir_);
	return p1 + (((plane.dot - nDotP1) / nDotDir) * dir_);
}
void CalculateCenterHalfWidth(std::vector<glm::vec3> points, glm::vec3& center_, float& halfWidth)
{
	float very_min_x = 10, very_max_x = -10, // default -10 to 10
		very_min_y = 10, very_max_y = -10,
		very_min_z = 10, very_max_z = -10;
	glm::vec3 xMinPoint = points[0], xMaxPoint = points[0],
		yMinPoint = points[0], yMaxPoint = points[0], zMinPoint = points[0], zMaxPoint = points[0];
	for (int i = 0; i < points.size(); ++i)
	{
		if (very_min_x > points[i].x)
		{
			very_min_x = points[i].x;
			xMinPoint = points[i];
		}
		if (very_max_x < points[i].x)
		{
			very_max_x = points[i].x;
			xMaxPoint = points[i];
		}
		if (very_min_y > points[i].y)
		{
			very_min_y = points[i].y;
			yMinPoint = points[i];
		}
		if (very_max_y < points[i].y)
		{
			very_max_y = points[i].y;
			yMaxPoint = points[i];
		}
		if (very_min_z > points[i].z)
		{
			very_min_z = points[i].z;
			zMinPoint = points[i];
		}
		if (very_max_z < points[i].z)
		{
			very_max_z = points[i].z;
			zMaxPoint = points[i];
		}
	}
	halfWidth = glm::max(glm::max((xMaxPoint.x - xMinPoint.x)  * 0.5f, (yMaxPoint.y - yMinPoint.y) * 0.5f),	(zMaxPoint.z - zMinPoint.z) * 0.5f);
	center_ = glm::vec3(xMaxPoint.x + xMinPoint.x, yMaxPoint.y + yMinPoint.y,
		zMaxPoint.z + zMinPoint.z) * 0.5f;
}
OctreeNode* BuildOctree(std::vector<Render*> obj, glm::vec3 center, float halfWidth_, int depth_, glm::vec3 color_)
{
	// Check indices, 300 triangles
	int num_triangle = CheckTriangleInBox(obj, center, halfWidth_, color_);
	if (num_triangle < 300)
		return nullptr;
	OctreeNode* pNode = new OctreeNode;
	pNode->center = center;
	pNode->halfWidth = halfWidth_;
	pNode->depth = depth_;

	glm::vec3 color = glm::vec3(0);
	if (depth_ == 0)
		color = glm::vec3(0, 0, 1);
	else if (depth_ == 1)
		color = glm::vec3(0, 1, 0);
	else if (depth_ == 2)
		color = glm::vec3(1, 0, 0);
	else if (depth_ == 3)
		color = glm::vec3(0.2, 0.2, 0.2);
	else if (depth_ == 4)
		color = glm::vec3(0.2, 0.6, 0.6);
	else if (depth_ == 5)
		color = glm::vec3(0.6, 0.2, 0.6);
	else if (depth_ == 6)
		color = glm::vec3(0.6, 0.6, 0.2);
	else if (depth_ == 7)
		color = glm::vec3(0.6, 0.8, 0.8);
	else if (depth_ == 8)
		color = glm::vec3(0.1, 0.9, 0.4);
	pNode->box_color = color;

	glm::vec3 offset(0);
	float step = halfWidth_ * 0.5f;
	for (int i = 0; i < 8; ++i)
	{
		offset.x = ((i & 1) ? step : -step);
		offset.y = ((i & 2) ? step : -step);
		offset.z = ((i & 4) ? step : -step);

		pNode->pChild[i] = BuildOctree(obj, center + offset, step, depth_ + 1, pNode->box_color);
	}
	return pNode;
}
int CheckTriangleInBox(std::vector<Render*> obj, glm::vec3 center, float halfWidth_, glm::vec3 color_)
{
	float xMax = center.x + halfWidth_, xMin = center.x - halfWidth_;
	float yMax = center.y + halfWidth_, yMin = center.y - halfWidth_;
	float zMax = center.z + halfWidth_, zMin = center.z - halfWidth_;
	int num_triangle = 0;
	
	for (unsigned i = 0; i < obj.size(); ++i)
	{
		Render* temp_obj = obj[i];
		for (unsigned j = 0; j < obj[i]->obj_indices.size(); j += 3)
		{
			glm::vec3 vert_1 = temp_obj->obj_vertices[temp_obj->obj_indices[j]] + temp_obj->position;
			glm::vec3 vert_2 = temp_obj->obj_vertices[temp_obj->obj_indices[j + 1]] + temp_obj->position;
			glm::vec3 vert_3 = temp_obj->obj_vertices[temp_obj->obj_indices[j + 2]] + temp_obj->position;
			bool vert_1_in = false;
			bool vert_2_in = false;
			bool vert_3_in = false;

			// If a point is inside a box, color.
			if (vert_1.x < xMax && vert_1.x > xMin && vert_1.y < yMax && vert_1.y > yMin && vert_1.z < zMax && vert_1.z > zMin)
			{
				vert_1_in = true;
				obj[i]->obj_colors[temp_obj->obj_indices[j]] = color_;
			}
			if (vert_2.x < xMax && vert_2.x > xMin && vert_2.y < yMax && vert_2.y > yMin && vert_2.z < zMax && vert_2.z > zMin)
			{
				vert_2_in = true;
				obj[i]->obj_colors[temp_obj->obj_indices[j + 1]] = color_;
			}
			if (vert_3.x < xMax && vert_3.x > xMin && vert_3.y < yMax && vert_3.y > yMin && vert_3.z < zMax && vert_3.z > zMin)
			{
				vert_3_in = true;
				obj[i]->obj_colors[temp_obj->obj_indices[j + 2]] = color_;
			}
			// But only treat as triangle if any point of triangle is inside a box
			if(vert_1_in || vert_2_in || vert_3_in)
				++num_triangle;
		}
	}
	return num_triangle;
}
float MergeTwoBoundingBox(HierarchyManager* obj_1, HierarchyManager* obj_2)
{
	glm::vec3 obj_1_max = obj_1->BVposition + obj_1->BVscale;
	glm::vec3 obj_1_min = obj_1->BVposition - obj_1->BVscale;
	glm::vec3 obj_2_max = obj_2->BVposition + obj_2->BVscale;
	glm::vec3 obj_2_min = obj_2->BVposition - obj_2->BVscale;

	glm::vec3 all_max = glm::max(obj_1_max, obj_2_max);
	glm::vec3 all_min = glm::min(obj_1_min, obj_2_min);

	return glm::distance(all_min, all_max);
}
void FindNodesToMerge(HierarchyManager** Nodes_, int num_obj, int& i, int& j)
{
	// Find two closest object
	float min_dist = static_cast<float>(INT_MAX);
	for (int index = 0; index < num_obj; ++index)
	{
		for (int index_2 = index; index_2 < num_obj; ++index_2)
		{
			if (index == index_2)
				continue;
			float bb_scale = MergeTwoBoundingBox(Nodes_[index], Nodes_[index_2]);
			if (bb_scale < min_dist)
			{
				min_dist = bb_scale;
				i = index;
				j = index_2;
			}
		}
	}
}
void ComputeBoundingVolume_BottomUp(HierarchyManager* obj_1, HierarchyManager* obj_2, glm::vec3& bv_position, glm::vec3& bv_scale, ShapeType shape_)
{
	// Calculate position and scale of two object
	glm::vec3 obj_1_max = obj_1->BVposition + obj_1->BVscale;
	glm::vec3 obj_1_min = obj_1->BVposition - obj_1->BVscale;
	glm::vec3 obj_2_max = obj_2->BVposition + obj_2->BVscale;
	glm::vec3 obj_2_min = obj_2->BVposition - obj_2->BVscale;

	glm::vec3 all_max = glm::max(obj_1_max, obj_2_max);
	glm::vec3 all_min = glm::min(obj_1_min, obj_2_min);

	if (shape_ == AABB)
	{
		bv_position = (all_max + all_min) * 0.5f;
		bv_scale = (all_max - all_min) * 0.5f;
	}
	else
	{
		bv_position = (all_max + all_min) * 0.5f;
		float radius = glm::distance(bv_position, all_max);
		bv_scale = glm::vec3(radius, radius, radius);
	}
}
glm::vec3 ComputeBoundingVolume(std::vector<glm::vec3>& points_, glm::vec3& position_, glm::vec3& scale_, ShapeType shape_)
{
	// calculate bounding volume scale and position
	if (points_.empty())
		return glm::vec3(0);
	float very_min_x = 10, very_max_x = -10, // default -10 to 10
		very_min_y = 10, very_max_y = -10,
		very_min_z = 10, very_max_z = -10;
	glm::vec3 xMinPoint = points_[0], xMaxPoint = points_[0],
		yMinPoint = points_[0], yMaxPoint = points_[0], zMinPoint = points_[0], zMaxPoint = points_[0];
	for (int i = 0; i < points_.size(); ++i)
	{
		if (very_min_x > points_[i].x)
		{
			very_min_x = points_[i].x;
			xMinPoint = points_[i];
		}
		if (very_max_x < points_[i].x)
		{
			very_max_x = points_[i].x;
			xMaxPoint = points_[i];
		}
		if (very_min_y > points_[i].y)
		{
			very_min_y = points_[i].y;
			yMinPoint = points_[i];
		}
		if (very_max_y < points_[i].y)
		{
			very_max_y = points_[i].y;
			yMaxPoint = points_[i];
		}
		if (very_min_z > points_[i].z)
		{
			very_min_z = points_[i].z;
			zMinPoint = points_[i];
		}
		if (very_max_z < points_[i].z)
		{
			very_max_z = points_[i].z;
			zMaxPoint = points_[i];
		}
	}

	float xDiff = glm::abs(xMaxPoint.x - xMinPoint.x);
	float yDiff = glm::abs(yMaxPoint.y - yMinPoint.y);
	float zDiff = glm::abs(zMaxPoint.z - zMinPoint.z);
	float maxDiff = glm::max(glm::max(xDiff, yDiff), zDiff);
	glm::vec3 xAxis = glm::vec3(xDiff, 0, 0);
	glm::vec3 yAxis = glm::vec3(0, yDiff, 0);
	glm::vec3 zAxis = glm::vec3(0, 0, zDiff);
	glm::vec3 longest_axis = glm::vec3(0);

	if (maxDiff == xDiff) {
		longest_axis = glm::vec3((very_max_x + very_min_x) * 0.5f, 0, 0);
	}
	else if (maxDiff == yDiff) {
		longest_axis = glm::vec3(0, (very_max_y + very_min_y) * 0.5f, 0);
	}
	else if (maxDiff == zDiff) {
		longest_axis = glm::vec3(0, 0, (very_max_z + very_min_z) * 0.5f);
	}

	if (shape_ == AABB)
	{
		scale_ = glm::vec3((xMaxPoint.x - xMinPoint.x)  * 0.5f,
			(yMaxPoint.y - yMinPoint.y) * 0.5f,
			(zMaxPoint.z - zMinPoint.z) * 0.5f);
		position_ = glm::vec3(xMaxPoint.x + xMinPoint.x, yMaxPoint.y + yMinPoint.y,
			zMaxPoint.z + zMinPoint.z) * 0.5f;
	}
	else
	{
		float radius = 0.f;
		glm::vec3 center(0);
		for (int i = 0; i < points_.size(); ++i)
			center += points_[i];
		center /= points_.size();
		for (int i = 0; i < points_.size(); ++i)
		{
			float dist = CalculateDistanceSq_(points_[i], center);
			if (radius < dist)
				radius = dist;
		}
		radius = glm::sqrt(radius);
		scale_ = glm::vec3(radius, radius, radius);
		position_ = center;
	}

	return longest_axis;
}
void Render::RenderBoundingBoxHierarchy(Camera* camera, Shader* shader, float aspect, glm::vec3 pos, glm::vec3 color_, HierarchyManager* tree, int currDepth)
{
	if (tree->depth == currDepth)
	{
		const static glm::vec3 up(0, 1, 0);

		glm::mat4 identity_translate(1.0);
		glm::mat4 identity_scale(1.0);
		glm::mat4 identity_rotation(1.0);

		glm::mat4 model = glm::translate(identity_translate, pos) * glm::scale(identity_scale, tree->BVscale) * glm::rotate(identity_rotation, rotation, up);
		glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
		glm::mat4 view = camera->GetViewMatrix();

		shader->SetMat4("model", model);
		shader->SetMat4("projection", projection);
		shader->SetMat4("view", view);
		shader->SetVec3("color", color_);

		glBindVertexArray(m_vao);
		glDrawElements(GL_LINE_STRIP, m_elementSize, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
	else
	{
		if(tree->left_)
			RenderBoundingBoxHierarchy(camera, shader, aspect, tree->left_->BVposition, color_, tree->left_, currDepth);
		if(tree->right_)
			RenderBoundingBoxHierarchy(camera, shader, aspect, tree->right_->BVposition, color_, tree->right_, currDepth);
	}
}
void Render::RenderOcTree(Camera* camera, Shader* shader, float aspect, OctreeNode* tree, int depth)
{
	for (int i = 0; i < 8; ++i)
	{
		if (tree)
		{
			const static glm::vec3 up(0, 1, 0);

			glm::mat4 identity_translate(1.0);
			glm::mat4 identity_scale(1.0);
			glm::mat4 identity_rotation(1.0);

			glm::vec3 scale_ = glm::vec3(tree->halfWidth, tree->halfWidth, tree->halfWidth);

			glm::vec3 position_ = tree->center;
			glm::mat4 model = glm::translate(identity_translate, position_) * glm::scale(identity_scale, scale_) * glm::rotate(identity_rotation, rotation, up);
			glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
			glm::mat4 view = camera->GetViewMatrix();

			shader->SetMat4("model", model);
			shader->SetMat4("projection", projection);
			shader->SetMat4("view", view);
			shader->SetVec3("color", tree->box_color);

			glBindVertexArray(m_vao);
			glDrawElements(GL_LINE_STRIP, m_elementSize, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
			if (tree->pChild[i])
			{
				RenderOcTree(camera, shader, aspect, tree->pChild[i], depth);
			}
		}
	}
}
void Render::RenderTriangle(Camera* camera, Shader* shader, float aspect, glm::vec3 pos)
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);

	glm::mat4 model = glm::translate(identity_translate, pos) * glm::scale(identity_scale, scale) * glm::rotate(identity_rotation, rotation, up);
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();

	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);

	GLint poly = 0;
	glGetIntegerv(GL_POLYGON_MODE, &poly);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(m_debug_vao);

	glDrawElements(GL_TRIANGLES, (GLsizei)m_elementSize, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, poly);
}
void Render::Describe(std::vector<glm::vec3>& vertices, std::vector<unsigned>& indices, std::vector<glm::vec2>& textures,
	std::vector<glm::vec3>& vnormals, bool isTexture)
{
	glGenVertexArrays(1, &m_vao);

	// Buffer of vertices, normals, textures, elements, bounding boxes
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &m_tbo);
	glGenBuffers(1, &m_ebo);
	glGenBuffers(1, &m_bbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, vnormals.size() * sizeof(glm::vec3), &vnormals[0], GL_STATIC_DRAW);

	if (isTexture)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_tbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	}

	const GLsizei vertex_size_stride = 0;
	constexpr GLint three_components_in_vertex_position = 3;
	constexpr GLint three_components_in_vertex_normal = 3;
	constexpr GLint two_components_in_vertex_texture_coordinates = 2;
	constexpr GLenum float_element_type = GL_FLOAT;
	constexpr GLboolean not_fixedpoint = GL_FALSE;
	const void* position_offset_in_vertex = reinterpret_cast<void*>(0);
	const void* normal_offset_in_vertex = reinterpret_cast<void*>(0);
	const void* texture_coordinates_offset_in_vertex = reinterpret_cast<void*>(0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, three_components_in_vertex_position, float_element_type, not_fixedpoint, vertex_size_stride, position_offset_in_vertex);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, three_components_in_vertex_normal, float_element_type, not_fixedpoint, vertex_size_stride, normal_offset_in_vertex);
	if (isTexture)
	{
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_tbo);
		glVertexAttribPointer(2, two_components_in_vertex_texture_coordinates, float_element_type, not_fixedpoint, vertex_size_stride, texture_coordinates_offset_in_vertex);
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_elementSize = (unsigned)indices.size() * sizeof(unsigned);
}
void Render::DescribeDebug(std::vector<glm::vec3>& vertices, std::vector<unsigned>& indices, std::vector<glm::vec3>& colors)
{
	glGenVertexArrays(1, &m_debug_vao);

	glGenBuffers(1, &m_debug_vbo);
	glGenBuffers(1, &m_debug_color);
	glGenBuffers(1, &m_debug_ebo);

	glBindVertexArray(m_debug_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_debug_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_debug_color);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

	const GLsizei vertex_size_stride = 0;
	constexpr GLint three_components_in_vertex_position = 3;
	constexpr GLint three_components_in_vertex_color = 3;
	constexpr GLenum float_element_type = GL_FLOAT;
	constexpr GLboolean not_fixedpoint = GL_FALSE;
	const void* position_offset_in_vertex = reinterpret_cast<void*>(0);
	const void* color_offset_in_vertex = reinterpret_cast<void*>(0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_debug_vbo);
	glVertexAttribPointer(0, three_components_in_vertex_position, float_element_type, not_fixedpoint, vertex_size_stride, position_offset_in_vertex);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_debug_color);
	glVertexAttribPointer(1, three_components_in_vertex_color, float_element_type, not_fixedpoint, vertex_size_stride, color_offset_in_vertex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_debug_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_elementSize = (unsigned)indices.size() * sizeof(unsigned);
}
void Render::RenderPartitionColor(Camera* camera, Shader* shader, float aspect, GLenum mode, glm::vec3 pos, OctreeNode* tree)
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);

	glm::vec3 scale_ = glm::vec3(tree->halfWidth, tree->halfWidth, tree->halfWidth);
	glm::vec3 color_ = glm::vec3(0.2f, 0, 0.9f);

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
	// send different color for each boxes
	shader->SetVec3("color", color_);
	if (shader->UseGeometry)
		shader->SetFloat("time", static_cast<float>(glfwGetTime()));

	glBindVertexArray(m_vao);

	if (isUsingTexture)
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
	else
		shader->SetVec3("color", color_);
	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);
	
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
void Render::RenderBoundingBox(Camera* camera, Shader* shader, float aspect, glm::vec3 pos, glm::vec3 color_)
{
	const static glm::vec3 up(0, 1, 0);

	glm::mat4 identity_translate(1.0);
	glm::mat4 identity_scale(1.0);
	glm::mat4 identity_rotation(1.0);

	glm::mat4 model = glm::translate(identity_translate, pos) * glm::scale(identity_scale, scale) * glm::rotate(identity_rotation, rotation, up);
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();

	shader->SetMat4("model", model);
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);
	shader->SetVec3("color", color_);

	glBindVertexArray(m_vao);
	glDrawElements(GL_LINE_STRIP, m_elementSize, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
glm::vec3 Render::CalculateBoundingBoxScale()
{
	float xScale = abs(xMax - xMin);
	float yScale = abs(yMax - yMin);
	float zScale = abs(zMax - zMin);
	return glm::vec3(xScale, yScale, zScale);
}

float Render::CalculateCenteroidSphere()
{
	float max_dst = 0;
	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		float dist = CalculateDistanceSq(obj_vertices[i]);
		if (max_dst < dist)
			max_dst = dist;
	}
	return glm::sqrt(max_dst);
}
float Render::CalculateRitterSphere()
{
	glm::vec3 x_min_point(0,0,0), x_max_point(0,0,0), 
		y_min_point(0, 0, 0), y_max_point(0, 0, 0), 
		z_min_point(0, 0, 0), z_max_point(0, 0, 0);
	float x_min = 0, x_max = 0, y_min = 0, y_max = 0, z_min = 0, z_max = 0;
	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		if (x_min > obj_vertices[i].x)
		{
			x_min = obj_vertices[i].x;
			x_min_point = obj_vertices[i];
		}
		if (x_max < obj_vertices[i].x)
		{
			x_max = obj_vertices[i].x;
			x_max_point = obj_vertices[i];
		}
		if (y_min > obj_vertices[i].y)
		{
			y_min = obj_vertices[i].y;
			y_min_point = obj_vertices[i];
		}
		if (y_max < obj_vertices[i].y)
		{
			y_max = obj_vertices[i].y;
			y_max_point = obj_vertices[i];
		}
		if (z_min > obj_vertices[i].z)
		{
			z_min = obj_vertices[i].z;
			z_min_point = obj_vertices[i];
		}
		if (z_max < obj_vertices[i].z)
		{
			z_max = obj_vertices[i].z;
			z_max_point = obj_vertices[i];
		}
	}
	float xDiff = CalculateDistanceSq(x_min_point, x_max_point);
	float yDiff = CalculateDistanceSq(y_min_point, y_max_point);
	float zDiff = CalculateDistanceSq(z_min_point, z_max_point);
	float MAX = glm::max(glm::max(xDiff, yDiff), zDiff);
	
	if (MAX == xDiff)
		sample_center = (x_min_point + x_max_point) * 0.5f;
	else if (MAX == yDiff)
		sample_center = (y_min_point + y_max_point) * 0.5f;
	else if (MAX == zDiff)
		sample_center = (z_min_point + z_max_point) * 0.5f;

	glm::vec3 center = sample_center;
	float radius_ = glm::sqrt(MAX) * 0.5f;

	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		glm::vec3 point = obj_vertices[i];
		float new_radius = CalculateDistance(center, point);
		if (radius_ < new_radius)
		{
			glm::vec3 direct_vec = point - center;
			glm::vec3 opposite_point = center - glm::normalize(direct_vec) * radius_;
			radius_ = CalculateDistance(opposite_point, point) * 0.5f;
			center = (opposite_point + point) * 0.5f;
		}
	}
	m_sphere.m_center = center;
	return radius_;
}
float Render::CalculateLarssonSphere()
{
	// TODO:
	glm::vec3 x_axis = glm::vec3(1, 0, 0);
	glm::vec3 y_axis = glm::vec3(0, 1, 0);
	glm::vec3 z_axis = glm::vec3(0, 0, 1);

	glm::vec3 FarthestPoint_0(0), ClosestPoint_0(0),
		FarthestPoint_1(0), ClosestPoint_1(0),
		FarthestPoint_2(0), ClosestPoint_2(0);

	FarthestFromVec(x_axis, FarthestPoint_0, ClosestPoint_0);
	FarthestFromVec(y_axis, FarthestPoint_1, ClosestPoint_1);
	FarthestFromVec(z_axis, FarthestPoint_2, ClosestPoint_2);

	glm::vec3 center = (FarthestPoint_0 + FarthestPoint_1 + FarthestPoint_2
		+ ClosestPoint_0 + ClosestPoint_1 + ClosestPoint_2) / 6.f;

	std::vector<glm::vec3> distances;
	float dist_0 = CalculateDistance(FarthestPoint_0, center);
	float dist_1 = CalculateDistance(FarthestPoint_1, center);
	float dist_2 = CalculateDistance(FarthestPoint_2, center);
	float dist_3 = CalculateDistance(ClosestPoint_0, center);
	float dist_4 = CalculateDistance(ClosestPoint_1, center);
	float dist_5 = CalculateDistance(ClosestPoint_2, center);
	float radius_ = glm::max(glm::max(glm::max(dist_0, dist_1), glm::max(dist_2, dist_3)), glm::max(dist_4, dist_5));

	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		glm::vec3 point = obj_vertices[i];
		float new_radius = CalculateDistance(center, point);
		if (radius_ < new_radius)
		{
			glm::vec3 direct_vec = point - center;
			glm::vec3 opposite_point = center - glm::normalize(direct_vec) * radius_;
			radius_ = CalculateDistance(opposite_point, point) * 0.5f;
			center = (opposite_point + point) * 0.5f;
		}
	}
	m_sphere.m_center = center;
	return radius_;
}
float Render::CalculatePCASphere()
{
	glm::vec3 mu(0, 0, 0);
	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		mu.x += obj_vertices[i].x;
		mu.y += obj_vertices[i].y;
		mu.z += obj_vertices[i].z;
	}
	mu /= (obj_vertices.size() - 1);

	glm::mat3x3 E_mat(0);
	E_mat = CoherentVec(mu);

	glm::vec3 eigenVec = EigenVector(E_mat);
	glm::vec3 FarthestPoint(0), ClosestPoint(0);

	FarthestFromVec(eigenVec, FarthestPoint, ClosestPoint);
	glm::vec3 center = (FarthestPoint + ClosestPoint) * 0.5f * eigenVec;

	float radius_ = CalculateDistance(FarthestPoint, ClosestPoint) * 0.5f;

	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		glm::vec3 point = obj_vertices[i];
		float new_radius = CalculateDistance(center, point);
		if (radius_ < new_radius)
		{
			glm::vec3 direct_vec = point - center;
			glm::vec3 opposite_point = center - glm::normalize(direct_vec) * radius_;
			radius_ = CalculateDistance(opposite_point, point) * 0.5f;
			center = (opposite_point + point) * 0.5f;
		}
	}
	m_sphere.m_center = center;
	return radius_;
}
glm::vec3 Render::CalculatePCAElipsoid()
{
	glm::vec3 mu(0, 0, 0);
	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		mu.x += obj_vertices[i].x;
		mu.y += obj_vertices[i].y;
		mu.z += obj_vertices[i].z;
	}
	mu /= (obj_vertices.size() - 1);

	glm::mat3x3 E_mat(0);
	E_mat = CoherentVec(mu);

	std::vector<glm::vec3> eigenVectors; // x, y, z axis
	EigenVector(E_mat, eigenVectors);

	glm::vec3 FarthestPoint_1(0), ClosestPoint_1(0), FarthestPoint_2(0), ClosestPoint_2(0),
		FarthestPoint_3(0), ClosestPoint_3(0);

	FarthestFromVec(eigenVectors[0], FarthestPoint_1, ClosestPoint_1);
	FarthestFromVec(eigenVectors[1], FarthestPoint_2, ClosestPoint_2);
	FarthestFromVec(eigenVectors[2], FarthestPoint_3, ClosestPoint_3);

	glm::vec3 center = (FarthestPoint_1 + ClosestPoint_1) * 0.5f * eigenVectors[0]
		+ (FarthestPoint_2 + ClosestPoint_2) * 0.5f * eigenVectors[1]
		+ (FarthestPoint_3 + ClosestPoint_3) * 0.5f * eigenVectors[2];

	float a = CalculateDistance(FarthestPoint_1, ClosestPoint_1) * 0.5f;
	float b = CalculateDistance(FarthestPoint_2, ClosestPoint_2) * 0.5f;
	float c = CalculateDistance(FarthestPoint_3, ClosestPoint_3) * 0.5f;

	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		glm::vec3 point = obj_vertices[i];
		float new_radius = CalculateDistance(center, point);
		float equation = (point.x * point.x) / (a * a) + (point.y * point.y) / (b * b);
		if (equation > 1) // outside of elipsoid
		{
			glm::vec3 direct_vec = point - center;
			glm::vec3 opposite_point = -point;
			glm::vec3 x_axis = glm::vec3(1, 0, 0);
			float cos_theta = glm::dot(x_axis, glm::normalize(direct_vec));
			if(glm::abs(cos_theta) > sqrt(2) * 0.5f)
				a = new_radius;
			else
				b = new_radius;
		}
	}
	glm::vec3 xyz_scale = glm::vec3(a, b, c);
	return xyz_scale;
}
glm::vec3 Render::CalculatePCAOBB()
{
	glm::vec3 mu(0, 0, 0);
	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		mu.x += obj_vertices[i].x;
		mu.y += obj_vertices[i].y;
		mu.z += obj_vertices[i].z;
	}
	mu /= (obj_vertices.size() - 1);

	glm::mat3x3 E_mat(0);
	E_mat = CoherentVec(mu);

	std::vector<glm::vec3> eigenVectors; // x, y, z axis
	EigenVector(E_mat, eigenVectors);

	glm::vec3 FarthestPoint_1(0), ClosestPoint_1(0), FarthestPoint_2(0), ClosestPoint_2(0),
		FarthestPoint_3(0), ClosestPoint_3(0);

	FarthestFromVec(eigenVectors[0], FarthestPoint_1, ClosestPoint_1);
	FarthestFromVec(eigenVectors[1], FarthestPoint_2, ClosestPoint_2);
	FarthestFromVec(eigenVectors[2], FarthestPoint_3, ClosestPoint_3);

	glm::vec3 center = (FarthestPoint_1 + ClosestPoint_1) * 0.5f * eigenVectors[0]
		+ (FarthestPoint_2 + ClosestPoint_2) * 0.5f * eigenVectors[1]
		+ (FarthestPoint_3 + ClosestPoint_3) * 0.5f * eigenVectors[2];

	float a = CalculateDistance(FarthestPoint_1, ClosestPoint_1) * 0.5f;
	float b = CalculateDistance(FarthestPoint_2, ClosestPoint_2) * 0.5f;
	float c = CalculateDistance(FarthestPoint_3, ClosestPoint_3) * 0.5f;

	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		float max = a;
		if (max < b)
			max = b;
		if (max < c)
			max = c;
		glm::vec3 point = obj_vertices[i];
		float new_radius = CalculateDistance(center, point);
		if(max < new_radius)
		{
			glm::vec3 direct_vec = point - center;
			glm::vec3 opposite_point = -point;
			if (max == a)
				a = new_radius;
			else if (max == b)
				b = new_radius;
			else if (max == c)
				c = new_radius;
		}
	}
	if (a < 0.65f)
		a = 0.65f;
	glm::vec3 xyz_scale = glm::vec3(a, b, c);
	return xyz_scale;
}
void Render::FarthestFromVec(glm::vec3 direction, glm::vec3& far_vec, glm::vec3& close_vec)
{
	float close_dist = glm::dot(obj_vertices[0], direction);
	float far_dist = close_dist;
	glm::vec3 farthestPoint(0), closestPoint(0);
	for (int i = 0; i < obj_vertices.size(); ++i)
	{
		float new_dist = glm::dot(obj_vertices[i], direction);
		if (new_dist > far_dist)
		{
			far_dist = new_dist;
			farthestPoint = obj_vertices[i];
		}
		else if (new_dist < close_dist)
		{
			close_dist = new_dist;
			closestPoint = obj_vertices[i];
		}
	}
	far_vec = farthestPoint;
	close_vec = closestPoint;
}
glm::vec3 Render::EigenVector(glm::mat3x3 mat_)
{
	// Rx
	glm::mat3x3 save_mat = mat_;
	int i = 0, j = 0, k = 0;
	float max_element = glm::max(glm::max(mat_[0][1], mat_[0][2]),mat_[1][2]);
	if (max_element == mat_[0][1]) {
		i = 0; j = 1, k = 2;
	}
	else if (max_element == mat_[0][2]) {
		i = 0; j = 2, k = 1;
	}
	else if (max_element == mat_[1][2]) {
		i = 1; j = 2, k = 0;
	}

	float Omega = (float)(save_mat[j][j] - save_mat[i][i]) / (2.f * (float)save_mat[i][j]);

	float tangent = 0.f;
	if (Omega < 0)
		tangent = -(Omega - glm::sqrt(Omega*Omega + 1));
	else
		tangent = -(Omega + glm::sqrt(Omega*Omega + 1));
	//float theta = std::atan(tangent);
	float cos_theta = 1 / (glm::sqrt(1 + tangent * tangent));
	float sin_theta = tangent / (glm::sqrt(1 + tangent * tangent));

	glm::vec3 result(0);
	glm::mat3x3 rot_mat(0), result_mat(0);
	
	// default i = 0, j = 1, k = 2
	rot_mat[i][j] = (cos_theta * cos_theta - sin_theta * sin_theta) * (float)save_mat[i][j]
						+ cos_theta * sin_theta * (float)(save_mat[i][i] - save_mat[j][j]);
	rot_mat[j][i] = (float)rot_mat[i][j];
	rot_mat[i][i] = cos_theta * cos_theta * (float)save_mat[i][i]
						- 2 * cos_theta * sin_theta * (float)save_mat[i][j]
						+ sin_theta * sin_theta * (float)save_mat[j][j];
	rot_mat[j][j] = sin_theta * sin_theta * (float)save_mat[i][i]
						+ 2 * cos_theta * sin_theta * (float)save_mat[i][j]
						+ cos_theta * cos_theta * (float)save_mat[j][j];
	rot_mat[i][k] = cos_theta * (float)save_mat[i][k] - sin_theta * (float)save_mat[j][k];
	rot_mat[k][i] = rot_mat[i][k];
	rot_mat[j][k] = sin_theta * (float)save_mat[i][k] + cos_theta * (float)save_mat[j][k];
	rot_mat[k][j] = rot_mat[j][k];
	rot_mat[k][k] = (float)save_mat[k][k];

	while ((float)save_mat[i][j] != 0 && (float)save_mat[i][k] != 0 && (float)save_mat[j][k] != 0)
	{
		save_mat = glm::transpose(rot_mat) * save_mat * rot_mat;
		//save_rot_mat.push_back(rot_mat);
	}
	/*glm::mat3x3 mat_v(0);
	for (int i = 0; i < save_rot_mat.size(); ++i)
	{
		mat_v *= save_rot_mat[i];
	}*/
	glm::vec3 row_0 = save_mat[0];//mat_v[0];
	glm::vec3 row_1 = save_mat[1];//mat_v[1];
	glm::vec3 row_2 = save_mat[2];//mat_v[2];
	float lambda_0 = (float)save_mat[0][0];
	float lambda_1 = (float)save_mat[1][1];
	float lambda_2 = (float)save_mat[2][2];
	float lambda_max = glm::max(glm::max(lambda_0, lambda_1), lambda_2);
	if (lambda_max == lambda_0)
		result = row_0;
	else if (lambda_max == lambda_1)
		result = row_1;
	else if (lambda_max == lambda_2)
		result = row_2;

	return result;
}
void Render::EigenVector(glm::mat3x3 mat_, std::vector<glm::vec3>& container)
{
	glm::mat3x3 save_mat = mat_;
	int i = 0, j = 0, k = 0;
	float max_element = glm::max(glm::max(mat_[0][1], mat_[0][2]), mat_[1][2]);
	if (max_element == mat_[0][1]) {
		i = 0; j = 1, k = 2;
	}
	else if (max_element == mat_[0][2]) {
		i = 0; j = 2, k = 1;
	}
	else if (max_element == mat_[1][2]) {
		i = 1; j = 2, k = 0;
	}

	float Omega = (float)(save_mat[j][j] - save_mat[i][i]) / (2.f * (float)save_mat[i][j]);

	float tangent = 0.f;
	if (Omega < 0)
		tangent = -(Omega - glm::sqrt(Omega*Omega + 1));
	else
		tangent = -(Omega + glm::sqrt(Omega*Omega + 1));
	//float theta = std::atan(tangent);
	float cos_theta = 1 / (glm::sqrt(1 + tangent * tangent));
	float sin_theta = tangent / (glm::sqrt(1 + tangent * tangent));

	glm::vec3 result(0);
	glm::mat3x3 rot_mat(0), result_mat(0);

	// default i = 0, j = 1, k = 2
	rot_mat[i][j] = (cos_theta * cos_theta - sin_theta * sin_theta) * (float)save_mat[i][j]
		+ cos_theta * sin_theta * (float)(save_mat[i][i] - save_mat[j][j]);
	rot_mat[j][i] = (float)rot_mat[i][j];
	rot_mat[i][i] = cos_theta * cos_theta * (float)save_mat[i][i]
		- 2 * cos_theta * sin_theta * (float)save_mat[i][j]
		+ sin_theta * sin_theta * (float)save_mat[j][j];
	rot_mat[j][j] = sin_theta * sin_theta * (float)save_mat[i][i]
		+ 2 * cos_theta * sin_theta * (float)save_mat[i][j]
		+ cos_theta * cos_theta * (float)save_mat[j][j];
	rot_mat[i][k] = cos_theta * (float)save_mat[i][k] - sin_theta * (float)save_mat[j][k];
	rot_mat[k][i] = rot_mat[i][k];
	rot_mat[j][k] = sin_theta * (float)save_mat[i][k] + cos_theta * (float)save_mat[j][k];
	rot_mat[k][j] = rot_mat[j][k];
	rot_mat[k][k] = (float)save_mat[k][k];

	while ((float)save_mat[i][j] != 0 && (float)save_mat[i][k] != 0 && (float)save_mat[j][k] != 0)
	{
		save_mat = glm::transpose(rot_mat) * save_mat * rot_mat;
	}
	// biggest 2
	glm::vec3 row_0 = save_mat[0];
	glm::vec3 row_1 = save_mat[1];
	glm::vec3 row_2 = save_mat[2];
	float lambda_0 = (float)save_mat[0][0];
	float lambda_1 = (float)save_mat[1][1];
	float lambda_2 = (float)save_mat[2][2];

	float lambda_max = glm::min(glm::min(lambda_0, lambda_1), lambda_2);
	if (lambda_max == lambda_0)
	{
		container.push_back(save_mat[0]);
		if (lambda_1 > lambda_2)
		{
			container.push_back(save_mat[1]);
			container.push_back(save_mat[2]);
		}
		else
		{
			container.push_back(save_mat[2]);
			container.push_back(save_mat[1]);
		}
	}	
	else if (lambda_max == lambda_1)
	{
		container.push_back(save_mat[1]);
		if (lambda_0 > lambda_2)
		{
			container.push_back(save_mat[0]);
			container.push_back(save_mat[2]);
		}
		else
		{
			container.push_back(save_mat[2]);
			container.push_back(save_mat[0]);
		}
	}
	else if (lambda_max == lambda_2)
	{
		container.push_back(save_mat[2]);
		if (lambda_0 > lambda_1)
		{

			container.push_back(save_mat[0]);
			container.push_back(save_mat[1]);
		}
		else
		{
			container.push_back(save_mat[1]);
			container.push_back(save_mat[0]);
		}
	}
}

bool Render::loadOBJ(const char * path, glm::vec3* middlePoint, int index, bool isUseTexture)
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
			faceNormals.clear();
			for (unsigned i = 0; i < obj_indices.size(); i += 3)
			{
				glm::vec3 vec1 = obj_vertices[obj_indices[i + 2]] - obj_vertices[obj_indices[i]];
				glm::vec3 vec2 = obj_vertices[obj_indices[i + 1]] - obj_vertices[obj_indices[i]];
				glm::vec3 faceNormal = glm::normalize(glm::cross(vec2, vec1));

				for (int j = 0; j < 3; ++j)
				{
					bool exist = false;
					auto it = faceNormals.equal_range(obj_indices[i + j]);
					for (auto iterator = it.first; iterator != it.second; ++iterator)
					{
						if (iterator->second == faceNormal)
						{
							exist = true;
							break;
						}
					}
					if (!exist)
						faceNormals.insert(std::make_pair(obj_indices[i + j], faceNormal));
				}
			}
			vertexNormals.clear();
			vertexNormals.resize(obj_vertices.size());
			for (auto face_it : faceNormals)
			{
				vertexNormals[face_it.first] += face_it.second;
			}
			for (auto & face_it : vertexNormals)
			{
				face_it = glm::normalize(face_it);
			}
			// Save min & max
			xMax = max_x;
			xMin = min_x;
			yMax = max_y;
			yMin = min_y;
			zMax = max_z;
			zMin = min_z;
			
			/* Set position and scale to default [-1, 1] */
			if (abs_max < glm::abs(max_x - min_x))
				abs_max = glm::abs(max_x - min_x);
			if (abs_max < glm::abs(max_y - min_y))
				abs_max = glm::abs(max_y - min_y);
			if (abs_max < glm::abs(max_z - min_z))
				abs_max = glm::abs(max_z - min_z);

			middlePoint[index] = glm::vec3((max_x + min_x) / (2 * abs_max),
				(max_y + min_y) / (2 * abs_max),
				(max_z + min_z) / (2 * abs_max));
			middlePoint_[index] = middlePoint[index]; // save
			max_absolute_val = abs_max;
			xMax /= (0.5f * abs_max);
			xMin /= (0.5f * abs_max);
			yMax /= (0.5f * abs_max);
			yMin /= (0.5f * abs_max);
			zMax /= (0.5f * abs_max);
			zMin /= (0.5f * abs_max);
			xMax -= (2.f * middlePoint[index].x);
			xMin -= (2.f * middlePoint[index].x);
			yMax -= (2.f * middlePoint[index].y);
			yMin -= (2.f * middlePoint[index].y);
			zMax -= (2.f * middlePoint[index].z);
			zMin -= (2.f * middlePoint[index].z);
			for (unsigned i = 0; i < obj_vertices.size(); ++i)
			{
				obj_vertices[i].x /= (0.5f * abs_max);
				obj_vertices[i].y /= (0.5f * abs_max);
				obj_vertices[i].z /= (0.5f * abs_max);
				obj_vertices[i] -= (2.f * middlePoint[index]);
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
			if (max_x < vertex.x)
				max_x = vertex.x;
			if (max_y < vertex.y)
				max_y = vertex.y;
			if (max_z < vertex.z)
				max_z = vertex.z;
			
			if (min_x > vertex.x)
				min_x = vertex.x;
			if (min_y > vertex.y)
				min_y = vertex.y;
			if (min_z > vertex.z)
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
	glBindVertexArray(m_vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_1);
	shader->SetInt("ourTexture1", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture_2);
	shader->SetInt("ourTexture2", 1);
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

glm::mat3x3 Render::CoherentVec(glm::vec3 mu_)
{
	glm::mat3x3 result(0);
	for (int i = 0; i < obj_vertices.size() - 1; ++i)
	{
		result[0][0] += (obj_vertices[i].x - mu_.x) * (obj_vertices[i].x - mu_.x); // xDiff * xDiff
		result[0][1] += (obj_vertices[i].x - mu_.x) * (obj_vertices[i].y - mu_.y); // xDiff * yDiff
		result[0][2] += (obj_vertices[i].x - mu_.x) * (obj_vertices[i].z - mu_.z); // xDiff * zDiff
		result[1][1] += (obj_vertices[i].y - mu_.y) * (obj_vertices[i].y - mu_.y); // yDiff * yDiff
		result[1][2] += (obj_vertices[i].y - mu_.y) * (obj_vertices[i].z - mu_.z); // ydiff * zDiff
		result[2][1] += (obj_vertices[i].z - mu_.z) * (obj_vertices[i].z - mu_.z); // zdiff * zDiff
	}

	result[1][0] = result[0][1];
	result[2][0] = result[0][2];
	result[2][1] = result[1][2];
	result /= obj_vertices.size();
	return result;
}
float CalculateDistanceSq_(const glm::vec3 a, const glm::vec3 b)
{
	return glm::abs((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}
float Render::CalculateDistanceSq(const glm::vec3 a, const glm::vec3 b)
{
	return glm::abs((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}
float Render::CalculateDistanceSq(const glm::vec3 a)
{
	return glm::abs((a.x)*(a.x) + (a.y)*(a.y) + (a.z)*(a.z));
}
float Render::CalculateDistance(const glm::vec3 a, const glm::vec3 b)
{
	return glm::sqrt(glm::abs((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z)));
}
float Render::CalculateDistance(const glm::vec3 a)
{
	return glm::sqrt(glm::abs((a.x)*(a.x) + (a.y)*(a.y) + (a.z)*(a.z)));
}