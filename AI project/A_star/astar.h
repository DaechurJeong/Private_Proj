#ifndef ASTAR
#define ASTAR

#include <vector>
#include <queue>
#include <tuple>
#include <limits>

//callback object for Astar
template <typename GraphType, typename AstarType>
class Callback {
protected:
	GraphType const& g;
public:
	Callback(GraphType const& _g) : g(_g) {}
	virtual ~Callback() {}
	virtual void OnIteration(AstarType const&) { }
	virtual void OnFinish(AstarType const&) { }
};

template <typename GraphType, typename Heuristic>
class Astar {
public:
	//				   PREV,    GOAL, F(x)
	typedef std::tuple<size_t, size_t, float, float> INFO;
	typedef std::tuple<size_t, size_t, float, float> ClosedlistData;
	class Comparison
	{
	public:
		int operator() (const INFO& p1, const INFO& p2) {
			return std::get<2>(p1) > std::get<2>(p2);
		}
	};
	Astar(GraphType const& _graph, Callback<GraphType, Astar>& cb) :
		graph(_graph),
		callback(cb),
		openlist(),
		closedlist(),
		solution(),
		evaluated(),
		gCost(),
		start_id(0),
		goal_id(0)
	{}

	std::vector<typename GraphType::Edge> search(size_t s, size_t g)
	{
		start_id = s;
		goal_id = g;
		openlist = std::priority_queue<INFO, std::vector<INFO>, Comparison>();
		closedlist.clear();
		solution.clear();
		evaluated.resize(g + 1, false);
		gCost.resize(g + 1, std::numeric_limits<float>::max());
		Heuristic heuristic;
		//heuristic from start to goal
		typename Heuristic::ReturnType h = heuristic(graph, graph.GetVertex(start_id), graph.GetVertex(goal_id));

		std::vector<typename GraphType::Edge> const& outedges = graph.GetOutEdges(start_id);

		// start point, so don't change initial start point, just goal point
		for (unsigned i = 0; i < outedges.size(); ++i)
		{
			goal_id = outedges[i].GetID2();
			float init_weight = outedges[i].GetWeight();
			h = heuristic(graph, graph.GetVertex(start_id), graph.GetVertex(goal_id));
			openlist.push(std::make_tuple(start_id, goal_id, init_weight + h, init_weight));
		}
		// Set first point to closed list
		closedlist.push_back(std::make_tuple(start_id, start_id, 0, 0));
		evaluated[0] = true;
		gCost[0] = 0;
		// Loop
		while (openlist.size() > 0) {
			callback.OnIteration(*this);
			// pop the top one from openlist
			INFO curr_node = openlist.top();
			openlist.pop();

			// find neighbor and calculate weight + h from here
			size_t prev_id = std::get<0>(curr_node);
			start_id = std::get<1>(curr_node);
			std::vector<typename GraphType::Edge> const& neighbor_edges = graph.GetOutEdges(start_id);
			// to subtract for exact gx for closed point
			h = heuristic(graph, graph.GetVertex(prev_id), graph.GetVertex(start_id));

			if (std::get<2>(curr_node) - h <= gCost[start_id])
			{
				closedlist.push_back(std::make_tuple(start_id, prev_id, std::get<2>(curr_node) - h, std::get<3>(curr_node)));
				gCost[start_id] = std::get<2>(curr_node) - h;
			}
			evaluated[start_id] = true;
			// find neighbor
			for (unsigned i = 0; i < neighbor_edges.size(); ++i)
			{
				goal_id = neighbor_edges[i].GetID2();
				// no duplicates, check
				if (evaluated[goal_id])
					continue;
				// put the neighbors' weight + h
				float new_gScore = gCost[start_id] + neighbor_edges[i].GetWeight();

				if (new_gScore < gCost[goal_id])
				{
					h = heuristic(graph, graph.GetVertex(start_id), graph.GetVertex(goal_id));
					openlist.push(std::make_tuple(start_id, goal_id, new_gScore + h, neighbor_edges[i].GetWeight()));
					gCost[goal_id] = new_gScore;
				}
			}
		}
		size_t step_id = g;
		size_t index = static_cast<size_t>(closedlist.size() - 1);
		float bst_weight = std::numeric_limits<float>::max();
		// push to solution
		while(true)
		{
			for (unsigned i = 0; i < closedlist.size(); ++i)
			{
				if (std::get<0>(closedlist[i]) == step_id) // found
				{
					if (bst_weight > std::get<2>(closedlist[i])
						&& gCost[step_id] == std::get<2>(closedlist[i]))
					{
						index = i;
						bst_weight = std::get<2>(closedlist[i]);
						break;
					}
				}
			}
			size_t curr = std::get<0>(closedlist[index]);
			size_t prev = std::get<1>(closedlist[index]);
			if (curr == 0 && prev == 0)
				break;
			float weight = std::get<3>(closedlist[index]);
			typename GraphType::Edge sol(curr, prev, weight);
			solution.push_back(sol);
			step_id = prev;
		}
		callback.OnFinish(*this);
		return solution;
	}
	////////////////////////////////////////////////////////////////////////
private:
	const GraphType& graph;
	Callback<GraphType, Astar>& callback;
	
	// PREV, GOAL, F(x)
	std::priority_queue<INFO, std::vector<INFO>, Comparison> openlist;
	// take curr Vertex, prev Vertex, gx
	std::vector<ClosedlistData> closedlist;
	std::vector<typename GraphType::Edge> solution;
	std::vector<bool> evaluated;
	std::vector<float> gCost;
	size_t                       start_id, goal_id;
};

#endif
