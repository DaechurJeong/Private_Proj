#include <iostream>
#include <vector>
#include <map>

using namespace std;
int firstMissingPositive(vector<int>& nums) {
    // num, idx?, shown
    //map<int, pair<int, bool>> table;
    map<int, int> table;
    for (int i = 0; i < nums.size(); ++i) {
        if (nums[i] > 0) {
            //table[i] = nums[i];
            table[nums[i]]++;
        }
    }

    int res = 1;
    while (true) {
        if (table[res] == 0) return res;
        res++;
    }

    return res;
}

int main(void)
{
    vector<int> input{ 1,2,0 };
    cout << firstMissingPositive(input) << endl;
	return 0;
}