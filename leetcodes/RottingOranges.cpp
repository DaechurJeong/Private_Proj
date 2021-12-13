#include <iostream>
#include <vector>
#include <queue>

int orangesRotting(std::vector<std::vector<int>>& grid) {
	int m = grid.size();
	int n = grid[0].size();
	std::queue<std::pair<int, int>> q;
	
	int empty = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (grid[i][j] == 2) {
				q.push(std::make_pair(i, j));
			}
			else if (grid[i][j] == 0) {
				++empty;
			}
		}
	}

	if (empty == m * n) return 0; // no oranges
	if (q.empty()) return -1; // no rotten oranges
	if (q.size() + empty == m * n) return 0; // no fresh oranges

	q.push(std::make_pair(-1, -1));

	int res = 0;
	while (!q.empty()) {
		// pop the top
		int x = q.front().first;
		int y = q.front().second;
		q.pop();
		if (x == -1 && y == -1) {
			res++;
			if (q.empty()) {
				break;
			}
			q.push({ -1,-1 });
			continue;
		}

		if (x - 1 >= 0 && grid[x - 1][y] == 1) {
			q.push(std::make_pair(x - 1, y));
			grid[x - 1][y] = 2;
		}
		if (x + 1 < grid.size() && grid[x + 1][y] == 1) {
			q.push(std::make_pair(x + 1, y));
			grid[x + 1][y] = 2;
		}
		if (y - 1 >= 0 && grid[x][y - 1] == 1) {
			q.push(std::make_pair(x, y - 1));
			grid[x][y - 1] = 2;
		}
		if (y + 1 < grid[x].size() && grid[x][y + 1] == 1) {
			q.push(std::make_pair(x, y + 1));
			grid[x][y + 1] = 2;
		}
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (grid[i][j] == 1) {
				return -1;
			}
		}
	}
	return res - 1;
}
int main(void)
{
	std::vector<int> row1{ 2,1,1 };
	std::vector<int> row2{ 1,1,0 };
	std::vector<int> row3{ 0,1,1 };

	/*std::vector<int> row1{ 2,1,1 };
	std::vector<int> row2{ 0,1,1 };
	std::vector<int> row3{ 1,0,1 };*/

	//std::vector<int> row1{ 1,2};

	std::vector<std::vector<int>> input{ row1,row2,row3 };
	std::cout << orangesRotting(input) << std::endl;

	return 0;
}