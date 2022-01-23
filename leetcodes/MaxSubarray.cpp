#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maxSubArray(vector<int>& nums) {
    int cursum = nums[0];
    int maxsum = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        if (cursum >= 0) {
            cursum += nums[i];
        }
        else
        {
            cursum = nums[i];
        }
        maxsum = max(maxsum, cursum);
    }
    return maxsum;
}

int main(void)
{
    vector<int> input{ -2,1,-3,4,-1,2,1,-5,4 };
    cout << maxSubArray(input) << endl;
	return 0;
}