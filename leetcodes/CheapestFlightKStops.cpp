//
//  CheapestPrice.cpp
//  Testing
//
//  Created by Daechul Jung on 12/29/21.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    unordered_map<int, vector<pair<int, int>>> mp;
    for (int i = 0; i < flights.size(); ++i) {
        mp[flights[i][0]].push_back(make_pair(flights[i][1], flights[i][2]));
    }
    // k += 1;
    // src, step, cost
    queue<tuple<int, int, int>> q;
    q.push(make_tuple(src, 0, 0));
    vector<int> cost(n, INT_MAX);
    cost[src] = 0;
    int minCost = INT_MAX;

    while (!q.empty()) {
        tuple<int, int, int> curr = q.front();
        q.pop();
        int curr_src = get<0>(curr);
        int curr_step = get<1>(curr);
        int curr_cost = get<2>(curr);
        if (curr_src == dst) minCost = min(minCost, curr_cost);
        if (curr_step == k + 1) continue;
        for (int i = 0; i < mp[curr_src].size(); ++i) {
            int dest = mp[curr_src][i].first;
            int cst = mp[curr_src][i].second;
            if (cost[dest] > curr_cost + cst) {
                cost[dest] = curr_cost + cst;
                q.push(make_tuple(dest, curr_step + 1, curr_cost + cst));
            }
        }
    }
    return (minCost == INT_MAX) ? -1 : minCost;
}

int main(void)
{
    vector<int> flight1{ 0,1,100 };
    vector<int> flight2{ 1,2,100 };
    vector<int> flight3{ 0,2,500 };

    vector<vector<int>> input{ flight1, flight2, flight3 };

    cout << findCheapestPrice(3, input, 0, 2, 1) << endl;

    return 0;
}