#include <iostream>
#include <vector>

using namespace std;

int searchInsert(vector<int>& nums, int target) {
	for (unsigned i = 0; i < nums.size(); ++i)
	{
		if (nums[i] >= target)
			return i;
	}
	return static_cast<int>(nums.size());
}

int main(void)
{
	vector<int> input;
	input.push_back(1);
	input.push_back(3);
	input.push_back(5);
	input.push_back(6);

	int result = searchInsert(input, 7);
	cout << result << endl;

	return 0;
}