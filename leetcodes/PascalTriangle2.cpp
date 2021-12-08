#include <iostream>
#include <vector>

std::vector<std::vector<int>> generate(int numRows) {
	std::vector<std::vector<int>> res;

	for (int i = 0; i < numRows; ++i) {
		std::vector<int> row;
		row.push_back(1);
		for (int j = 1; j < i; ++j) {
			row.push_back(res[i - 1][j - 1] + res[i - 1][j]);
		}
		if (i > 0) {
			row.push_back(1);
		}
		res.push_back(row);
	}
	return res;
}

int main(void) {
	std::vector<std::vector<int>> res = generate(5);

	for (auto each : res) {
		std::cout << "[";
		for (auto idx : each) {
			std::cout << idx << ",";
		}
		std::cout << "]" << std::endl;
	}

	return 0;
}