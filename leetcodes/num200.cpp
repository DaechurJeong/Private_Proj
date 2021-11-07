#include <iostream>
#include <vector>

void countIsland(std::vector<std::vector<char>>& grid, int i, int j, int num_row, int num_col)
{
	if (grid[i][j] == '0') return;
	if (grid[i][j] == '1')
	{
		grid[i][j] = '0';
		if (i < num_row - 1)
			countIsland(grid, i + 1, j, num_row, num_col);
		if(j < num_col - 1)
			countIsland(grid, i, j + 1, num_row, num_col);
		if (i > 0)
			countIsland(grid, i - 1, j, num_row, num_col);
		if (j > 0)
			countIsland(grid, i, j - 1, num_row, num_col);
	}
}
int numIslands(std::vector<std::vector<char>>& grid)
{
	if (grid.empty())
		return 0;
	int num_island = 0;
	for (unsigned row = 0; row < grid.size(); ++row)
	{
		for (unsigned col = 0; col < grid[row].size(); ++col)
		{
			if (grid[row][col] == '1')
			{
				num_island++;
				countIsland(grid, row, col, (int)grid.size(), (int)grid[row].size());
			}
		}
	}
	return num_island;
}
int main(void)
{
	std::vector<char> row(5, '0');
	std::vector<std::vector<char>> map(4, row);

	map[0][0] = '1';
	map[1][0] = '1';
	map[0][1] = '1';
	map[1][1] = '1';
	map[2][2] = '1';
	map[3][3] = '1';
	map[3][4] = '1';

	int res = numIslands(map);

	std::cout << res << std::endl;

	return 0;
}