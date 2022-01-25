#include <iostream>
#include <vector>

using namespace std;

int maxProfit(vector<int>& prices) {
    int res = 0;
    bool isGoingUp = false;
    int buyPrice = 0;
    for (int i = 1; i < prices.size(); ++i) {
        if (prices[i] > prices[i - 1]) {
            res += (prices[i] - prices[i - 1]);
        }
    }
    return res;
}

int main(void)
{
	vector<int> input{ 6,1,3,2,4,7 };
	cout << maxProfit(input) << endl;
	return 0;
}