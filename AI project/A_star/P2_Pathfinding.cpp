#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

AStarPather::AStarPather()
{

}
bool AStarPather::initialize()
{
	MapNodes.reserve(MAX_MAP_SIZE * MAX_MAP_SIZE);
	m_heap.reserve(MAX_MAP_SIZE * MAX_MAP_SIZE);

	Callback cb = std::bind(&AStarPather::Map_preprocessing, this);
	Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    return true; // return false if any errors actually occur, to stop engine initialization
}
void AStarPather::Map_preprocessing()
{
	MapNodes.resize(terrain.get()->get_map_height() * terrain.get()->get_map_width());
}
void AStarPather::shutdown()
{
	MapNodes.clear();
	m_heap.clear();
}

AStarPather::Nodes* AStarPather::GridPosToNodes(GridPos& g_pos)
{
	return &MapNodes[g_pos.row * terrain->get_map_width() + g_pos.col];
}
GridPos AStarPather::NodesToGridPos(Nodes* m_node)
{
	int index_offset = (int)(m_node - &MapNodes[0]);
	
	GridPos result;
	result.row = index_offset / terrain->get_map_width();
	result.col = index_offset % terrain->get_map_width();
	return result;
}
PathResult AStarPather::compute_path(PathRequest &request)
{
	if (request.newRequest)
	{
		start = terrain->get_grid_position(request.start);
		goal = terrain->get_grid_position(request.goal);
		
		if (terrain->is_wall(goal) || !terrain->is_valid_grid_position(goal))
		{
			std::cout << "Invalid goal position\n";
			return PathResult::IMPOSSIBLE;
		}

		for (auto& init : MapNodes)
		{
			init.parent.col = -1;
			init.parent.row = -1;
			init.fx = 0; init.gx = 0;
			init.is_closed = -1;
		}

		m_heap.clear();
		Nodes* start_node = GridPosToNodes(start);
		m_heap.insertKey(start_node);
		start_node->is_closed = 0;   // open list
	}

	while (m_heap.getSize())
	{
		// Pop cheapest node off
		Nodes* min_node = m_heap.extractMin();
		m_heap.pop();

		// If node is the Goal Node, then path found
		if (NodesToGridPos(min_node) == goal)
		{
			// push the path into the vector
			PushBstSolution(min_node, request);

			return PathResult::COMPLETE;
		}
		// Check neighboring child nodes
		for (int i = 0; i < 8; ++i)
		{
			GridPos neighbor_of_curr = GetNeighbor(min_node, i);
			// Is it valid position? Is it wall?
			if (!terrain->is_valid_grid_position(neighbor_of_curr) || terrain->is_wall(neighbor_of_curr))
				continue;
			Nodes* neighbor_node = CheckNeighbor(neighbor_of_curr, i);

			if (neighbor_node)
			{
				// compute cost
				float new_gx = min_node->gx;
				if (i % 2)
					new_gx += SQRT_OF_TWO; // diagonal
				else
					new_gx += 1.f;

				float new_fx = new_gx + CalculateHeuristicCost(neighbor_of_curr, request) * request.settings.weight;

				// If child node isn't on Open or Closed list, put it on Open List
				if (neighbor_node->is_closed == -1)
				{
					neighbor_node->fx = new_fx;
					neighbor_node->gx = new_gx;
					neighbor_node->parent = NodesToGridPos(min_node);
					m_heap.insertKey(neighbor_node);
					neighbor_node->is_closed = 0;

					if(request.settings.debugColoring)
						terrain->set_color(neighbor_of_curr, Colors::Aqua);
				}
				else// Open list or Closed list
				{
					// If child node is on Open or Closed list, and min_node is cheaper,
					// take the old expensive one off both lists and put min_node on Open list
					if (new_fx < neighbor_node->fx)
					{
						neighbor_node->fx = new_fx;
						neighbor_node->gx = new_gx;
						neighbor_node->parent = NodesToGridPos(min_node);
						if (neighbor_node->is_closed == 1)
							m_heap.insertKey(neighbor_node);
						else
							m_heap.decreaseKey(neighbor_node);
						neighbor_node->is_closed = 0;
						if (request.settings.debugColoring)
							terrain->set_color(neighbor_of_curr, Colors::Aqua);
					}
				}
			}
		}
		if (request.settings.debugColoring)
			terrain->set_color(NodesToGridPos(min_node), Colors::Yellow);
		// Place parent node on the Closed List
		min_node->is_closed = 1;
		// If taken too much time this frame, abort and go next frame
		if (request.settings.singleStep)
			return PathResult::PROCESSING;
	}
    // Open List empty, thus no path possible (Return fail)
	if (m_heap.empty())
		return PathResult::IMPOSSIBLE;

    return PathResult::COMPLETE;
}

void AStarPather::PushBstSolution(Nodes* final_node, PathRequest& request)
{
	GridPos node_as_gridpos = NodesToGridPos(final_node);

	if (!request.settings.rubberBanding)
	{
		while (1)
		{
			request.path.push_front(terrain->get_world_position(node_as_gridpos));
			node_as_gridpos = GridPosToNodes(node_as_gridpos)->parent;

			if (node_as_gridpos.col < 0 || node_as_gridpos.row < 0)
				break;
		}
	}
	if (request.settings.rubberBanding && request.settings.smoothing)
	{
		Rubberbanding(request);
		InsertPath(request);
		Smoothing(request);
	}
	else if (request.settings.rubberBanding)
	{
		Rubberbanding(request);
	}
	else if (request.settings.smoothing)
	{
		Smoothing(request);
	}
}
void AStarPather::Rubberbanding(PathRequest& request)
{
	// From goal to start, go backward
	GridPos start_ = goal;
	GridPos mid_ = GridPosToNodes(start_)->parent;
	GridPos end_ = GridPosToNodes(mid_)->parent;
	
	request.path.push_front(terrain->get_world_position(start_));
	while (start_ != start) // last node, start_->parent == start
	{
		if (AbleToReduce(start_, end_)) // able to reduce!
		{
			if (end_.col < 0 || end_.row < 0)
			{
				request.path.push_front(terrain->get_world_position(start));
				break;
			}
			mid_ = end_;
			end_ = GridPosToNodes(end_)->parent;
			GridPosToNodes(goal)->parent = mid_;
		}
		else // Wall in there, disable to reduce
		{
			if (end_.col < 0 || end_.row < 0)
			{
				request.path.push_front(terrain->get_world_position(start));
				break;
			}
			request.path.push_front(terrain->get_world_position(mid_));
			start_ = mid_;
			mid_ = end_;
			end_ = GridPosToNodes(end_)->parent;
		}
	}
}
void AStarPather::InsertPath(PathRequest& request)
{
	auto start_ = request.path.begin();
	auto end_ = start_;
	++end_;

	while (end_ != request.path.end())
	{
		// check once, then next step
		PreventOutrange(request, start_);
		start_ = end_;
		++end_;
	}
}
void AStarPather::PreventOutrange(PathRequest& request, WaypointList::iterator& pivot)
{
	auto end_it = pivot;
	++end_it;

	float distance = Vec3::DistanceSquared(*pivot, *end_it);

	float min_dist = terrain->mapSizeInWorld / terrain->get_map_height() * 1.5f;

	if (distance > min_dist * min_dist) {
		Vec3 loc = *pivot;
		loc += (*end_it - *pivot) * 0.5f;
		request.path.insert(end_it, loc);

		auto aft_pivot = pivot;
		++aft_pivot;

		PreventOutrange(request, pivot);  // partition
		PreventOutrange(request, aft_pivot);
	}
	else
		return;
}
bool AStarPather::AbleToReduce(GridPos& start_, GridPos& end_)
{
	int xDiff = abs(end_.col - start_.col);
	int yDiff = abs(end_.row - start_.row);
	int xDirection = 0, yDirection = 0;
	GridPos checkPoint = start_;

	if (xDiff)
	{
		if (end_.col - start_.col == xDiff)
			xDirection = 1;
		else
			xDirection = -1;
	}
	if (yDiff)
	{
		if (end_.row - start_.row == yDiff)
			yDirection = 1;
		else
			yDirection = -1;
	}
	if (xDiff != 0 && yDiff != 0)
	{
		for (int i = 0; i <= xDiff; ++i)
		{
			for (int i = 0; i <= yDiff; ++i)
			{
				if (checkPoint.row < 0 || checkPoint.col < 0) // invalid
					break;
				else if (terrain->is_wall(checkPoint))
					return false;
				checkPoint.row += yDirection;
			}
			checkPoint.col += xDirection;
			checkPoint.row = start_.row;
		}
	}
	else if (xDiff == 0) 
	{
		for (int i = 0; i <= yDiff; ++i)
		{
			if (checkPoint.row < 0) // invalid
				break;
			 else if (terrain->is_wall(checkPoint))
				return false;
			checkPoint.row += yDirection;
		}
	}
	else if (yDiff == 0)
	{
		for (int i = 0; i <= xDiff; ++i)
		{
			if (checkPoint.col < 0) // invalid
				break;
			else if (terrain->is_wall(checkPoint))
				return false;
			checkPoint.col += xDirection;
		}
	}
	return true;
}
Vec3 AStarPather::CatmullRom_Spline(Vec3& point_1,
	Vec3& point_2, Vec3& point_3, Vec3& point_4, float s)
{
	Vec3 output_point = point_1 * (-0.5f*s*s*s + s * s - 0.5f*s)
		+ point_2 * (1.5f*s*s*s - 2.5f*s*s + 1.0f)
		+ point_3 * (-1.5f*s*s*s + 2.0f*s*s + 0.5f*s)
		+ point_4 * (0.5f*s*s*s - 0.5f*s*s);
	return output_point;
}
void AStarPather::Smoothing(PathRequest& request)
{
	if (!request.path.size())
		return;
	size_t size_ = request.path.size();
	auto begin = request.path.begin();
	auto prev_mid = begin;
	if((int)size_ > 1)
		++prev_mid;
	auto aft_mid = prev_mid;
	if((int)size_ > 2)
		++aft_mid;
	auto end = aft_mid;
	if ((int)size_ > 3)
		++end;

	WaypointList::iterator points_loc = aft_mid;
	Vec3 out_point;
	while (*prev_mid != terrain->get_world_position(goal))
	{
		std::vector<Vec3> vec_list;
		vec_list.push_back(CatmullRom_Spline(*begin, *prev_mid, *aft_mid, *end, 0.25f));
		vec_list.push_back(CatmullRom_Spline(*begin, *prev_mid, *aft_mid, *end, 0.5f));
		vec_list.push_back(CatmullRom_Spline(*begin, *prev_mid, *aft_mid, *end, 0.75f));
		request.path.insert(points_loc, vec_list.begin(), vec_list.end());

		begin = prev_mid;
		prev_mid = aft_mid;
		aft_mid = end;
		if (*end != terrain->get_world_position(goal))
			++end;
		points_loc = aft_mid;
	}
}
GridPos AStarPather::GetNeighbor(Nodes* currNode, int index)
{
	GridPos currPos = NodesToGridPos(currNode);

	if (index == 0) { // upper
		currPos.row += 1;
	}
	else if (index == 1) { // up-right diagonal
		currPos.col += 1;
		currPos.row += 1;
	}
	else if (index == 2) { // right
		currPos.col += 1;
	}
	else if (index == 3) { // bot-right diagonal
		currPos.row -= 1;
		currPos.col += 1;
	}
	else if (index == 4) { // bottom
		currPos.row -= 1;
	}
	else if (index == 5) { // bot-left diagonal
		currPos.row -= 1;
		currPos.col -= 1;
	}
	else if (index == 6) { // left
		currPos.col -= 1;
	}
	else if (index == 7) { // up-left diagonal
		currPos.row += 1;
		currPos.col -= 1;
	}
	return currPos;
}
AStarPather::Nodes* AStarPather::CheckNeighbor(GridPos& neighborPos, int index)
{
	GridPos neighbor_v_check = neighborPos;
	GridPos neighbor_h_check = neighborPos;
	
	if (index == 1) { // up-right diagonal
		neighbor_h_check.row -= 1;
		neighbor_v_check.col -= 1;
	}
	else if (index == 3) { // bottom-right diagonal
		neighbor_v_check.col -= 1;
		neighbor_h_check.row += 1;
	}
	else if (index == 5) { // bottom-left diagonal
		neighbor_v_check.col += 1;
		neighbor_h_check.row += 1;
	}
	else if (index == 7) { // left-up diagonal
		neighbor_v_check.col += 1;
		neighbor_h_check.row -= 1;
	}
	// Check the neighbor is valid & not wall
	if ((!terrain->is_valid_grid_position(neighbor_v_check) || terrain->is_wall(neighbor_v_check))
		|| (!terrain->is_valid_grid_position(neighbor_h_check) || terrain->is_wall(neighbor_h_check)))
		return nullptr;

	// Check finish. return original position
	return GridPosToNodes(neighborPos);
}
float AStarPather::CalculateHeuristicCost(GridPos& currPos, PathRequest& request)
{
	Heuristic currStatus = request.settings.heuristic;
	float xDiff = static_cast<float>(abs(currPos.row - goal.row));
	float yDiff = static_cast<float>(abs(currPos.col - goal.col));

	if (currStatus == Heuristic::EUCLIDEAN)
	{
		// sqrt((xDiff)^2 + (yDiff)^2)
		return (sqrt(static_cast<float>(pow(abs(xDiff), 2)
			+ pow(abs(yDiff), 2))));
	}
	else if (currStatus == Heuristic::OCTILE)
	{
		// min(xDiff, yDiff) * sqrt(2) + max(xDiff, yDiff) - min(xDiff, yDiff)
		return std::min(xDiff, yDiff) * SQRT_OF_TWO_MINUS_ONE + std::max(xDiff, yDiff);
	}
	else if (currStatus == Heuristic::CHEBYSHEV)
	{
		// max(xDiff, yDiff)
		return static_cast<float>(std::max(abs(xDiff), abs(yDiff)));
	}
	else if (currStatus == Heuristic::MANHATTAN)
	{
		// xDiff + yDiff
		return static_cast<float>(abs((xDiff - goal.row)) + abs((yDiff)));
	}

	// default Octile
	return std::min(xDiff, yDiff) * SQRT_OF_TWO_MINUS_ONE + std::max(xDiff, yDiff);
}
void AStarPather::swap(Nodes& a, Nodes& b)
{
	Nodes temp = a;
	a = b;
	b = temp;
}