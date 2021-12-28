#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int maxPerformance(int n, vector<int>& speed, vector<int>& efficiency, int k) {

	vector<pair<int, int>> pairs;
	for (int i = 0; i < n; ++i)
	{
		pairs.push_back({ efficiency[i], speed[i] });
	}

	// sort by efficiency
	sort(pairs.begin(), pairs.end());

	priority_queue<int, vector<int>, greater<int>> pq;
	long long ret = 0, sum = 0, mod = 1e9 + 7;
	for (int i = pairs.size() - 1; i >= 0; --i)
	{
		pq.push(pairs[i].second);
		sum += pairs[i].second;
		if (pq.size() > k) {
			sum -= pq.top();
			pq.pop();
		}
		ret = max(ret, sum * pairs[i].first);
	}
	return ret % mod;
}

int main(void)
{
	vector<int> speed{ 2,10,3,1,5,8 };
	vector<int> efficiency{ 5,4,3,9,7,2 };

	cout << maxPerformance(6, speed, efficiency, 2) << endl;

	return 0;
}