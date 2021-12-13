#include <iostream>
#include <vector>

std::vector<int> Spiral(std::vector<std::vector<int>> input)
{
	std::vector<int> res;
	int rowStart = 0;
	int rowEnd = input.size() - 1;
	int columnStart = 0;
	int columnEnd = input[0].size() - 1;

	while (rowStart <= rowEnd && columnStart <= columnEnd)
	{
		for (int i = columnStart; i <= columnEnd; ++i) {
			res.push_back(input[rowStart][i]);
		}
		++rowStart;
		for (int i = rowStart; i <= rowEnd; ++i) {
			res.push_back(input[i][columnEnd]);
		}
		--columnEnd;
		if (rowStart <= rowEnd && columnStart <= columnEnd) {
			for (int i = columnEnd; i >= columnStart; --i) {
				res.push_back(input[rowEnd][i]);
			}
			--rowEnd;
			for (int i = rowEnd; i >= rowStart; --i) {
				res.push_back(input[i][columnStart]);
			}
			++columnStart;
		}
	}
	return res;
}

int main(void)
{
	std::vector<int> row1{ 1,0,5,4,3 };
	std::vector<int> row2{ 3,2,1,0,5 };
	std::vector<int> row3{ 6,2,3,9,7 };
	std::vector<std::vector<int>> input{row1, row2, row3};

	std::vector<int> res = Spiral(input);
	for (int i = 0; i < res.size(); ++i)
	{
		std::cout << res[i] << " ";
	}
	std::cout << std::endl;

	return 0;
}