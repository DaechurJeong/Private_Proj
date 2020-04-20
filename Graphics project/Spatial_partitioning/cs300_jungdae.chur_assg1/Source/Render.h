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
class Render;

enum BVHType {
	TOP_DOWN,
	BOTTOM_UP,
	OCTREE,
	BSP_TREE,
};
enum ShapeType {
	AABB,
	SPHERE,
};

struct HierarchyManager {
	HierarchyManager* left_ = 0;
	HierarchyManager* right_ = 0;
	Render* objects;
	std::vector<glm::vec3> points;

	glm::vec3 BVscale;
	glm::vec3 BVposition;
	int depth;

	int numObjects;
};
struct OctreeNode {
	OctreeNode* pChild[8];
	Render* pObjList;

	glm::vec3 center;
	float halfWidth;
	glm::vec3 box_color;

	int depth;
};
enum PointPos {
	POINT_FRONT,
	POINT_BEHIND,
	POINT_ON,
};
enum PolygonPos {
	COPLANAR,
	FRONT,
	BEHIND,
	STRADDLING,
};
struct BSPNode {
	BSPNode() {};
	BSPNode(std::vector<std::vector<glm::vec3>> triangle, std::vector<Render*> obj, std::vector<int> obj_number, std::vector<std::vector<unsigned>> indiceList);

	BSPNode* front;
	BSPNode* back;

	std::vector<std::vector<glm::vec3>> m_triangle_list;

	int depth;
};
struct Plane {
	Plane() { 
		normal = glm::vec3(0);
		dot = 0;
	}
	glm::vec3 normal; // plane normal
	float dot; // d = dot(n, p) for a given point p on the plane
};

void AddObject(Render* obj_list, int index, glm::vec3* mid_point,
	Render* obj_list_debug, Render* obj_bounding_box, Render* obj_aabb, ShapeType shape_,
	std::vector<Render*>& aabb_obj_vec, std::vector<Render*>& real_obj_vec, std::vector<glm::vec3>& all_point,
	std::vector<std::vector<glm::vec3>>& points_in_triangle,
	std::vector<std::vector<unsigned>>& indices_in_triangle,
	std::vector<int>& triangle_obj_number);

void TopDownBVTree(HierarchyManager** tree, std::vector<glm::vec3> point_, int depth_, ShapeType shape_);
HierarchyManager* BottomUpBVTree(std::vector<Render*> obj, ShapeType shape_);
OctreeNode* BuildOctree(std::vector<Render*> obj, glm::vec3 center, float halfWidth_, int depth_, glm::vec3 color_);
BSPNode* BuildBSPTree(std::vector<Render*> obj, std::vector<std::vector<glm::vec3>>& triangleList, std::vector<int> obj_number, std::vector<std::vector<unsigned>> indiceList, int depth_);

// For BSPTree
Plane ComputePlane(glm::vec3 a, glm::vec3 b, glm::vec3 c);
glm::vec3 IntersectEdgeAgainstPlane(glm::vec3 p1, glm::vec3 p2, Plane plane);
Plane PickSplittingPlane(std::vector<std::vector<glm::vec3>>& tri_indice_list);
int ClassifyPointToPlane(glm::vec3 point, Plane plane);
int ClassifyTriangleToPlane(std::vector<glm::vec3> triangle_indice, Plane plane);

// For OcTree
int CheckTriangleInBox(std::vector<Render*> obj, glm::vec3 center, float halfWidth_, glm::vec3 color_);
void CalculateCenterHalfWidth(std::vector<glm::vec3> points, glm::vec3& center_, float& halfWidth);

// For TopDown & BottomUp
void FindNodesToMerge(HierarchyManager** Nodes_, int num_obj, int& i, int& j);
void ComputeBoundingVolume_BottomUp(HierarchyManager* obj_1, HierarchyManager* obj_2, glm::vec3& bv_position, glm::vec3& bv_scale, ShapeType shape_);
float MergeTwoBoundingBox(HierarchyManager* obj_1, HierarchyManager* obj_2); // return distance

glm::vec3 ComputeBoundingVolume(std::vector<glm::vec3>& points_, glm::vec3& position_, glm::vec3& scale_, ShapeType shape_);
float CalculateDistanceSq_(const glm::vec3 a, const glm::vec3 b);
class Render
{
	unsigned m_vao, m_vbo, m_ebo, m_ubo, m_tbo, normalBuffer, m_bbo;
	unsigned m_debug_vao, m_debug_vbo, m_debug_ebo, m_debug_color;
	unsigned m_elementSize, m_fnormSize;
	unsigned m_texture_1, m_texture_2;
	int width, height;
	
	bool isUsingTexture;

	glm::mat3x3 CoherentVec(glm::vec3 mu_);
	glm::vec3 EigenVector(glm::mat3x3 mat_);
	void EigenVector(glm::mat3x3 mat_, std::vector<glm::vec3>& container);
	void FarthestFromVec(glm::vec3 direction, glm::vec3& far_vec, glm::vec3& close_vec);
public:
	Render();
	~Render();

	void Rendering(Camera* camera, Shader* shader, float aspect, GLenum mode, glm::vec3 pos, glm::vec3 color_);
	void RenderBoundingBox(Camera* camera, Shader* shader, float aspect, glm::vec3 pos, glm::vec3 color_);
	void RenderBoundingBoxHierarchy(Camera* camera, Shader* shader, float aspect, glm::vec3 pos, glm::vec3 color_, HierarchyManager* tree, int currDepth);
	void RenderOcTree(Camera* camera, Shader* shader, float aspect, OctreeNode* tree, int depth);
	void RenderPartitionColor(Camera* camera, Shader* shader, float aspect, GLenum mode, glm::vec3 pos, OctreeNode* tree);
	void RenderTriangle(Camera* camera, Shader* shader, float aspect, glm::vec3 pos);

	void Describe(std::vector<glm::vec3>& vertices, std::vector<unsigned>& indices, std::vector<glm::vec2>& textures,
		std::vector<glm::vec3>& vnormals, bool isTexture);
	void DescribeDebug(std::vector<glm::vec3>& vertices, std::vector<unsigned>& indices, std::vector<glm::vec3>& colors);

	bool loadOBJ(const char* path, glm::vec3* middlePoint, int index, bool isUseTexture);
	bool loadPPM(const char* path, std::vector<glm::vec3>& values_);
	void SendTextureInfo(Shader* shader);
	void DescribeTexture();

	float CalculateDistanceSq(const glm::vec3 a, const glm::vec3 b);
	float CalculateDistanceSq(const glm::vec3 a);
	float CalculateDistance(const glm::vec3 a, const glm::vec3 b);
	float CalculateDistance(const glm::vec3 a);

	glm::vec3 CalculateBoundingBoxScale();
	// for spheres, return radius
	float CalculateCenteroidSphere(); 
	float CalculateRitterSphere();
	float CalculateLarssonSphere();
	float CalculatePCASphere();
	glm::vec3 CalculatePCAElipsoid(); // same func OBB & PCAElipsoid
	glm::vec3 CalculatePCAOBB();

	int whichProjection; // 0 = planar, 1 = cylindrical, 2 = spherical
	int calculateByNormal; // 0 = vertex, 1 = normal
	std::vector<glm::vec2> textureUV;
	std::vector<glm::vec3> g_spec_data;
	std::vector<glm::vec3> g_diff_data;

	std::vector<glm::vec3> obj_vertices;
	std::multimap<int, glm::vec3> faceNormals;
	std::vector<glm::vec3> vertexNormals;
	std::vector<unsigned> obj_indices;
	std::vector<glm::vec3> obj_colors; // for partitioning color

	glm::vec3 position, scale, color;
	glm::vec3 middlePoint_[20];
	glm::vec3 sample_center;
	float rotation;
	float xMax, xMin, yMax, yMin, zMax, zMin, max_absolute_val;

	struct Sphere {
		glm::vec3 m_center;
		float m_radius;
	};
	
	enum BoundingVolumeType {
		AABB,
		Centeroid,
		Ritter,
		Larsson,
		PCA_sphere,
		PCA_ellipsoid,
		OBB,
	};
	BoundingVolumeType m_BVtype;
	Sphere m_sphere;
};