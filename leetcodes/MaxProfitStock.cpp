#include <iostream>
#include <vector>

int maxProfit(std::vector<int>& prices) {
	int min = prices[0], res = 0;
	for (int i = 1; i < prices.size(); ++i) {
		if (min > prices[i]) {
			min = prices[i];
		}
		else if (prices[i] - min > res) {
			res = prices[i] - min;
		}
	}
	return res;
}

int main(void)
{
	std::vector<int> input{ 7,1,5,3,6,4 };
	//std::vector<int> input{ 7,6,4,3,1 };

	std::cout << maxProfit(input) << std::endl;

	return 0;
}