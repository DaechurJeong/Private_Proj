#include <iostream>
#include <vector>

void rotate(std::vector<std::vector<int>>& matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = i; j < matrix[0].size(); j++)
			std::swap(matrix[i][j], matrix[j][i]);
	}

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0, k = matrix[0].size() - 1; j < k; j++, k--)
			std::swap(matrix[i][j], matrix[i][k]);
	}
}

int main(void)
{
	std::vector<std::vector<int>> input;
	std::vector<int> row;
	row.push_back(1);
	row.push_back(2);
	row.push_back(3);
	input.push_back(row);

	std::vector<int> row1;
	row1.push_back(4);
	row1.push_back(5);
	row1.push_back(6);
	input.push_back(row1);

	std::vector<int> row2;
	row2.push_back(7);
	row2.push_back(8);
	row2.push_back(9);
	input.push_back(row2);

	rotate(input);
	for (unsigned i = 0; i < input.size(); ++i)
	{
		for (unsigned j = 0; j < input[i].size(); ++j)
		{
			std::cout << input[i][j] << " ";
		}
		std::cout << std::endl;
	}
}