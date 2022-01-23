#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

void helper(vector<int>& curr, vector<int>& nums, int depth, set<vector<int>>& ansSet)
{
    if (depth >= nums.size()) {
        return;
    }
    curr.push_back(nums[depth]);
    ansSet.insert(curr);
    helper(curr, nums, depth + 1, ansSet);
    curr.pop_back();
    helper(curr, nums, depth + 1, ansSet);
}
vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    vector<int> curr;
    sort(nums.begin(), nums.end());
    set<vector<int>> ansSet;
    vector<vector<int>> res;
    res.push_back({});
    helper(curr, nums, 0, ansSet);
    for (auto it : ansSet) {
        res.push_back(it);
    }
    return res;
}

int main(void)
{
    vector<int> input{ 1,2,2 };
    vector<vector<int>> output = subsetsWithDup(input);

    for (auto a : output) {
        for (auto b : a) {
            cout << b << ", ";
        }
        cout << endl;
    }
	return 0;
}