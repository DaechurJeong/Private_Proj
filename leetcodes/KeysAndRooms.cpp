#include <iostream>
#include <vector>
#include <queue>

using namespace std;

bool canVisitAllRooms(vector<vector<int>>& rooms) {
	vector<bool> visited(rooms.size(), false);
	queue<int> q;
	visited[0] = true;
	for (int i = 0; i < rooms[0].size(); ++i)
	{
		q.push(rooms[0][i]);
	}

	while (!q.empty())
	{
		int curr = q.front();
		q.pop();
		if (!visited[curr]) {
			visited[curr] = true;
			for (int i = 0; i < rooms[curr].size(); ++i) {
				if (!visited[rooms[curr][i]]) {
					q.push(rooms[curr][i]);
				}
			}
		}
	}
	if (find(visited.begin(), visited.end(), false) != visited.end()) {
		return false;
	}
	return true;
}

int main(void)
{
	//vector<vector<int>> input{ {1}, {2}, {3}, {} };
	vector<vector<int>> input{ {1,3}, {3,0,1}, {2}, {0} };

	string output = canVisitAllRooms(input) ? "TRUE" : "FALSE";

	cout << output << endl;

	return 0;
}