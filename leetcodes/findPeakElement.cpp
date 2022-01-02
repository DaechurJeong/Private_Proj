#include <iostream>
#include <vector>

int findPeakElement(vector<int>& nums) 
{
    int left = 0;
    int right = nums.size() - 1;
    while(left < right) {
        int mid = (left + right) / 2;
        if(nums[mid] > nums[mid + 1]) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

int main(void)
{
	std::vector input{1,2,3,1};

	std::cout << findPeakElement(input) << std::endl;

	return 0;
}