#include <iostream>
#include <vector>

std::vector<int> getRow(int rowIndex) {
	if (rowIndex == 0) {
		std::vector<int> res(1,1);
		return res;
	}
	std::vector<int> res(rowIndex + 1, 1);
	std::vector<int> prev = getRow(rowIndex - 1);

	for (int i = 1; i < rowIndex; ++i) {
		res[i] = prev[i - 1] + prev[i];
	}
	return res;
}

int main(void)
{
	std::vector<int> res = getRow(5);

	for (int i = 0; i < res.size(); ++i)
	{
		std::cout << res[i] << " ";
	}
	std::cout << std::endl;

	return 0;
}