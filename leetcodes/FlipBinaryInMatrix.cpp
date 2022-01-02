#include <iostream>
#include <vector>

using namespace std;

bool isFlipsPossible(vector<vector<int>> matrix)
{
	int n = matrix.size(), m = matrix[0].size();
	// for the rows first
	for (int j = 0; j < m; ++j) {
		if (matrix[0][j]) {
			for (int i = 0; i < n; ++i){
				matrix[i][j] ^= 1;
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		if (matrix[i][0]) {
			for (int j = 0; j < m; ++j) {
				matrix[i][j] ^= 1;
			}
		}
	}
	bool allZero = 1;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			allZero &= matrix[i][j] == 0;
		}
	}
	return allZero;
}

int main(void)
{
	/*vector<vector<int>> input{
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{1,1,0,1,1},
		{0,0,1,0,0}
	};*/
	vector<vector<int>> input{
		{0,0,1,0},
		{1,1,0,1},
		{0,0,1,0},
		{1,1,0,1},
	};

	string res = isFlipsPossible(input) ? "TRUE" : "FALSE";

	cout << res << endl;

	return 0;
}