#include <iostream>
#include <vector>

using namespace std;

int removeElement(vector<int>& nums, int val) 
{
	int count = 0;
	for (unsigned i = 0; i < nums.size(); ++i)
	{
		if (nums[i] != val)
		{
			nums[i] = val;
			++count;
		}
	}
	return count;
}

int main(void)
{
	vector<int> input;

	input.push_back(0);
	input.push_back(1);
	input.push_back(2);
	input.push_back(2);
	input.push_back(3);
	input.push_back(0);
	input.push_back(4);
	input.push_back(2);

	int output = removeElement(input, 2);

	cout << output << endl;

	return 0;
}