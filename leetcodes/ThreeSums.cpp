#include <iostream>
#include <vector>

using namespace std;

bool find_sum_of_two(vector<int> arr, int required_sum, int currIdx)
{
    // find sum of two in the sorted array, with two indexs in the loop
    for(int i = currIdx, j = (arr.size() - 1); i < (arr.size() - 1);) {
        int sum = arr[i] + arr[j];
        if(sum == required_sum) {
            return true;
        } else if(sum < required_sum) {
            ++i;
        } else if (sum > required_sum) {
            --j;
        }
        if(i > j) {
            return false;
        }
    }
    return false;
}

bool find_sum_of_three(vector<int> arr, int required_sum) {
    if(arr.empty()) return false;
    // sort it
    sort(arr.begin(), arr.end());
    
    // in for loop, find sum of two for required_sum - arr[i]
    for(int i = 0; i < arr.size() - 2; ++i)
    {
        if(find_sum_of_two(arr, required_sum - arr[i], i)) {
            return true;
        }
    }
    
    return false;
}

int main(void)
{
    std::vector<int> input {3,7,1,2,8,4,5};
    
    cout << find_sum_of_three(input, 21) << endl;
    
	return 0;
}
