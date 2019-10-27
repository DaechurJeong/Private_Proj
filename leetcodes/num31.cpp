#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void nextPermutation(vector<int>& nums) {
	int id1 = 0, id2 = 0;
	if (nums.size() >= 2) {
		for (int i = nums.size() - 2; i >= 0; i--)
		{
			if (nums[i] < nums[i + 1])
			{
				id1 = i;
				break;
			}
		}

		for (int i = nums.size() - 1; i >= 0; i--)
		{
			if (nums[id1] < nums[i])
			{
				id2 = i;
				break;
			}
		}
		swap(nums[id1], nums[id2]);
		sort(nums.begin() + id1 + 1, nums.end());
		if (id1 == 0 && id2 == 0)
			sort(nums.begin(), nums.end());
	}
}

int main(void)
{
	return 0;
}