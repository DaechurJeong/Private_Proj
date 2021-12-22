//
//  MinDifference.cpp
//  Testing
//
//  Created by Daechul Jung on 12/22/21.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int minDifference(vector<int>& nums)
{
    int sz = nums.size();
    if(sz <= 3) return 0;
    sort(nums.begin(), nums.end());
    int case0 = nums[sz - 1] - nums[3];
    int case1 = nums[sz - 2] - nums[2];
    int case2 = nums[sz - 3] - nums[1];
    int case3 = nums[sz - 4] - nums[0];
    return min(min(case0, case1), min(case2, case3));
}
int main(void)
{
    vector<int> input{6,6,0,1,1,4,6};
    std::cout << minDifference(input) << std::endl;
    return 0;
}
