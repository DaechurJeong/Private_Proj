#include <iostream>
#include <vector>

void rotate(std::vector<std::vector<int>>& matrix) {
	int size = (int)matrix.size();

	for (int i = 0; i < size / 2; ++i)
	{
		for (int j = i; j < size - 1 - i; ++j)
		{
			int tmp = matrix[i][j];

			matrix[i][j] = matrix[size - 1 - j][i];
			matrix[size - 1 - j][i] = matrix[size - 1 - i][size - 1 - j];
			matrix[size - 1 - i][size - 1 - j] = matrix[j][size - 1 - i];
			matrix[j][size - 1 - i] = tmp;
		}
	}
}

int main(void)
{
	std::vector<int> row1{1,2,3};
	std::vector<int> row2{4,5,6};
	std::vector<int> row3{7,8,9};

	std::vector<std::vector<int>> input{row1, row2, row3};

	std::cout << "Before rotate" << std::endl;
	for (auto input_idx : input) {
		for (auto row : input_idx) {
			std::cout << row << " ";
		}
		std::cout << std::endl;
	}

	rotate(input);

	std::cout << "After rotate" << std::endl;
	for (auto input_idx : input) {
		for (auto row : input_idx) {
			std::cout << row << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}