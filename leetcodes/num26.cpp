#include <iostream>
#include <vector>

using namespace std;

int removeDuplicates(vector<int>& nums)
{
	int i = 0;
	for (auto num : nums)
		if (i < 1 || nums[i - 1] < num)
			nums[i++] = num;
	return i;
	/*int cmp = nums[0];
	int count = 1;

	for (vector<int>::iterator it = nums.begin(); it != nums.end() - 1; ++it)
	{
		if (cmp == (*it))
		{
			nums.erase(it);
			continue;
		}
		else
		{
			++count;
			cmp = (*it);
		}
	}
	return count;*/
}

int main(void)
{
	vector<int> input;
	input.push_back(0);
	input.push_back(0);
	input.push_back(0);
	input.push_back(1);
	input.push_back(1);
	input.push_back(2);
	input.push_back(2);
	input.push_back(2);
	input.push_back(3);
	input.push_back(3);

	int result = removeDuplicates(input);

	cout << result << endl;

	return 0;
}