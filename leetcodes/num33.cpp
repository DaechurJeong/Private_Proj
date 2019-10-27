#include <iostream>
#include <vector>

using namespace std;

int search(vector<int>& nums, int target) {
	int start = 0;
	int end = nums.size() - 1;
	int mid = (start + end) / 2;
	if (nums.size() == 0)
		return -1;
	while (start < end)
	{
		if (nums[mid] == target)
			return mid;
		if (nums[start] <= nums[mid])
		{
			if (nums[mid] > target && nums[start] <= target)
				end = mid - 1;
			else
				start = mid + 1;
		}
		else {
			if (nums[mid] < target && nums[end] >= target)
				start = mid + 1;
			else
				end = mid - 1;
		}
		mid = (start + end) / 2;
	}
	if (nums[start] == target)
		return start;
	return -1;
}
int main(void)
{
	vector<int> input;
	for (int i = 4; i < 8; ++i)
		input.push_back(i);
	for (int i = 0; i < 3; ++i)
		input.push_back(i);
	int target = 0;

	int output = search(input, target);

	cout << output << endl;

	return 0;
}