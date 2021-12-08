#include <iostream>
#include <vector>
#include <set>
#include <map>

int singleNumber(std::vector<int>& nums) {
	std::map<int, int> map;
	for (int i = 0; i < nums.size(); ++i) {
		if (map.find(nums[i]) != map.end()) {
			++map[nums[i]];
		}
		else {
			map.emplace(nums[i], 1);
		}
	}
	for (auto val : map) {
		if (val.second == 1)
			return val.first;
	}
	return 0;
}

int main(void)
{
	std::vector<int> input{ 4,1,2,1,2 };

	std::cout << singleNumber(input) << std::endl;

	return 0;
}