#include <unordered_map>
#include <iostream>
#include <vector>

using namespace std;

void dfs(vector<vector<int>>& isConnected, vector<bool>& visited, int startP)
{
    if (visited[startP]) return;
    visited[startP] = true;
    for (int j = 0; j < isConnected.size(); ++j) {
        if (startP == j) continue;
        if(isConnected[startP][j] == 1) {
            dfs(isConnected, visited, j);
        }
    }
}
int findCircleNum(vector<vector<int>>& isConnected) {
    // dfs
    // for loop isConnected
    // visited, to reduce time

    vector<bool> visited(isConnected.size(), false);
    int count = 0;
    for (int i = 0; i < visited.size(); ++i) {
        if (!visited[i]) {
            ++count;
            dfs(isConnected, visited, i);
        }
    }
    return count;
}

int main(void)
{
    vector<vector<int>> input{
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
        {0,1,0,1,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,0,1,0,0,0,1,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,1,0,0},
        {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,1,0,0},
        {0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    };
    cout << findCircleNum(input) << endl;

	return 0;
}