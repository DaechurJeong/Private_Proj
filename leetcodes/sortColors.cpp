#include <iostream>
#include <vector>

void swapByIdx(std::vector<int>& nums, int i, int j) {
	int tmp = nums[i];
	nums[i] = nums[j];
	nums[j] = tmp;
}
void sortColors(std::vector<int>& nums) {
	int left = 0, mid = 0, right = (int)nums.size() - 1;
	while(mid <= right) {
		if (nums[mid] == 2) {
			swapByIdx(nums, mid, right);
			--right;
		}
		else if (nums[mid] == 0) {
			swapByIdx(nums, mid, left);
			++left;
			++mid;
		}
		else if (nums[mid] == 1) {
			++mid;
		}
	}
}

int main(void)
{
	std::vector<int> input{ 2,0,1 };

	sortColors(input);

	for (unsigned int i = 0; i < input.size(); ++i) {
		std::cout << input[i] << " ";
	}

	return 0;
}