#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector<int> frequencySort(vector<int>& nums) {
    map<int, int> freq;
    for (int i = 0; i < nums.size(); ++i)
    {
        freq[nums[i]]++;
    }
    sort(nums.begin(), nums.end(), [&](int a, int b) {
        return freq[a] == freq[b] ? a > b : freq[a] < freq[b];
        });

    return nums;
}

int main(void)
{
    vector<int> input{ 1,1,2,2,2,3 };
    vector<int> output = frequencySort(input);
    for (int i = 0; i < output.size(); ++i)
    {
        cout << output[i] << " ";
    }
	return 0;
}