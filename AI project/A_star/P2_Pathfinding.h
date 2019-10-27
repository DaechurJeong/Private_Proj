#pragma once
#include "MinHeap.h"

#define MAX_MAP_SIZE 40
#define SQRT_OF_TWO 1.414213f
#define SQRT_OF_TWO_MINUS_ONE 0.414213f

class AStarPather
{
public:

	AStarPather();

    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);

	struct Nodes {
		GridPos parent;
		float fx; // total cost
		float gx; // given cost
		char is_closed; // -1 No set, 0 open-list, 1 closed-list
	};
	struct KeyHashTable
	{
		intptr_t operator()(const Nodes* value) const {
			return intptr_t(value);
		}
	};
	struct Comparison
	{
		bool operator()(const Nodes* a, const Nodes* b) const
		{
			if (a->fx == b->fx)
				return a->gx < b->gx;
			return a->fx > b->fx;
		}
	};

private:
	std::vector<Nodes> MapNodes;
	GenericMinHeap<Nodes*, KeyHashTable, Comparison> m_heap;

	GridPos start;
	GridPos goal;

	void Map_preprocessing(void);
	
	// functions
	Nodes* GridPosToNodes(GridPos& g_pos);
	GridPos NodesToGridPos(Nodes* m_node);
	void PushBstSolution(Nodes* final_node, PathRequest& request);
	float CalculateHeuristicCost(GridPos& currPos, PathRequest& request);

	void Rubberbanding(PathRequest& request);
	void InsertPath(PathRequest& request);
	void PreventOutrange(PathRequest& request, WaypointList::iterator& pivot);
	void Smoothing(PathRequest& request);
	Vec3 CatmullRom_Spline(Vec3& point_1,
		Vec3& point_2, Vec3& point_3, Vec3& point_4, float s);
	bool AbleToReduce(GridPos& start_, GridPos& end_);

	GridPos GetNeighbor(Nodes* currNode, int index); // index distinguish the neighbor direction
	Nodes* CheckNeighbor(GridPos& neighborPos, int index);

	// helper functions
	void swap(Nodes& a, Nodes& b);
};
