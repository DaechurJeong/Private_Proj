#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maxProfit(vector<int>& prices, int fee) {
	int cash = 0;
	int hold = -prices[0];
	for (int i = 0; i < prices.size(); ++i) {
		// buy
		cash = max(cash, hold + prices[i] - fee);
		// sell
		hold = max(hold, cash - prices[i]);
	}
	return cash;
}

int main(void)
{
	vector<int> input{ 1,3,2,8,4,9 };
	cout << maxProfit(input, 2) << endl;

	return 0;
}