#include "Render.h"

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
	//Render new_obj = alksdfjlkasjdf;
	//Nodes_[index]->objects = new_obj;
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
float CalculateDistanceSq_(const glm::vec3 a, const glm::vec3 b)
{
	return glm::abs((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}