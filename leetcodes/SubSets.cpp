#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> ret;
void helper(vector<int>& curr, vector<int>& nums, int depth)
{
    ret.push_back(curr);
    for (int i = depth; i < nums.size(); ++i) {
        curr.push_back(nums[i]);
        helper(curr, nums, i + 1);
        curr.pop_back();
    }
}
vector<vector<int>> subsets(vector<int>& nums) {
    vector<int> curr;
    helper(curr, nums, 0);
    return ret;
}

int main(void)
{
    vector<int> input{ 1,2,3 };
    vector<vector<int>> output = subsets(input);

    for (auto a : output) {
        for (auto b : a) {
            cout << b << ", ";
        }
        cout << endl;
    }
	return 0;
}