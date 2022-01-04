#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

long long maxPoints(vector<vector<int>>& points) 
{
	long long res = 0;
	int m = points.size();
	int n = points[0].size();

	vector<vector<long long>> dp(m, vector<long long>(n, INT_MIN));

	//for the first row
	for (int i = 0; i < n; ++i) {
		dp[0][i] = points[0][i];
	}
	for (int i = 1; i < m; ++i) {
		long long currMax = INT_MIN;
		for (int j = 0; j < n; ++j) {
			currMax = max(currMax, dp[i - 1][j] + j);
			dp[i][j] = max(dp[i][j], currMax + points[i][j] - j);
		}
		currMax = INT_MIN;
		for (int j = n - 1; j >= 0; --j) {
			currMax = max(currMax, dp[i - 1][j] - j);
			dp[i][j] = max(dp[i][j], currMax + points[i][j] + j);
		}
	}
	for (int j = 0; j < n; ++j) {
		res = max(res, dp[m - 1][j]);
	}
	return res;
}

int main(void)
{
	/*vector<vector<int>> input{
		{1,2,3},
		{1,5,1},
		{3,1,1}
	};*/
	/*vector<vector<int>> input{
		{1,5}, {2,3}, {4,2}
	};*/
	vector<vector<int>> input{
		{0,3,0,4,2},
		{5,4,2,4,1},
		{5,0,0,5,1},
		{2,0,1,0,3}
	};

	cout << maxPoints(input) << endl;

	return 0;
}