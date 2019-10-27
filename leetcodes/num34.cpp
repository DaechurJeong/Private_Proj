#include <iostream>
#include <vector>

using namespace std;

vector<int> searchRange(vector<int>& nums, int target)
{
	vector<int> res;
	int i;
	for (i = 0; i < nums.size(); i++) {
		if (nums[i] == target) {
			res.push_back(i);
			break;
		}
	}
	for (int j = nums.size() - 1; j >= i; j--) {
		if (nums[j] == target) {
			res.push_back(j);
			break;
		}
	}
	if (res.size() == 0)
		return { -1,-1 };
	return res;
}
int main(void)
{
	vector<int> input;
	input.push_back(5);
	input.push_back(7);
	input.push_back(7);
	input.push_back(8);
	input.push_back(8);
	input.push_back(10);

	vector<int> result = searchRange(input, 6);

	for (int i = 0; i < result.size(); ++i)
		cout << result[i] << endl;

	return 0;
}