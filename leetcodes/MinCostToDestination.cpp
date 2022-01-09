#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

struct compare {
	bool operator() (const tuple<int, int, int, int> t1, const tuple<int, int, int, int> t2)
	{
		return get<3>(t1) > get<3>(t2);
	}
};
int minCost(int maxTime, vector<vector<int>>& edges, vector<int>& passingFees) 
{
	// time, src, dst, totalFee
	int n = passingFees.size();
	priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, compare> pq;
	vector<int>dis(n + 1000, INT_MAX);

	int final_dst = passingFees.size() - 1;

	for (int i = 0; i < edges.size(); ++i) {
		int time = edges[i][2];
		int src = edges[i][0];
		if (src == 0) { // initial
			if (time <= maxTime) {
				pq.push(make_tuple(time, src, edges[i][1], passingFees[src]));
			}
		}
	}
	if (pq.empty()) return -1;
	int totalFee = 0;
	while(!pq.empty()) {
		tuple<int, int, int, int> curr = pq.top();
		pq.pop();

		int time = get<0>(curr);
		if (time > maxTime) continue;
		totalFee = get<3>(curr);
		
		int dst = get<2>(curr);
		if (final_dst == dst) return totalFee + passingFees[dst];

		for (int i = 0; i < edges.size(); ++i) {
			if (dst == edges[i][0]) {
				if (time + edges[i][2] <= maxTime && dis[dst] > time + edges[i][2]) {
					dis[dst] = time + edges[i][2];
					pq.push(make_tuple(time + edges[i][2], edges[i][0], edges[i][1], totalFee + passingFees[dst]));
				}
			}
		}
	}
	return -1;
}

int main(void)
{
	vector<vector<int>> edges{
		{0,1,10},
		{1,2,10},
		{2,5,10},
		{0,3,1},
		{3,4,10},
		{4,5,15}
	};
	vector<int> passing{ 5,1,2,20,20,3 };
	/*vector<vector<int>> edges{
		{0, 1, 100}
	};
	vector<int> passing{ 2,5 };*/

	cout << minCost(30, edges, passing) << endl;

	return 0;
}