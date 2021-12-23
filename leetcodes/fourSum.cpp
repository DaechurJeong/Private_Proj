#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) 
{
	int ret = 0;
	unordered_map<int, int> map;
	for (auto num1 : nums1) {
		for (auto num2 : nums2) {
			map[num1 + num2]++;
		}
	}
	for (auto num3 : nums3) {
		for (auto num4 : nums4) {
			auto it = map.find(-(num3 + num4));
			if (it != map.end()) {
				ret += it->second;
			}
		}
	}
	return ret;
}

int main(void)
{
	vector<int> input1{ 1, 2 };
	vector<int> input2{ -2, -1 };
	vector<int> input3{ -1, 2 };
	vector<int> input4{ 0, 2 };

	cout << fourSumCount(input1, input2, input3, input4) << endl;

	return 0;
}