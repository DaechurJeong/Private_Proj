#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> merge(vector<vector<int>>& intervals) {
    vector<vector<int>> res;
    sort(intervals.begin(), intervals.end(), [&](vector<int> a, vector<int> b) {
        return a[0] < b[0];
        });
    for (vector<int> each : intervals) {
        // empty OR not overlapped
        if (res.empty() || res.back()[1] < each[0]) {
            res.push_back(each);
        }
        else {
            res.back()[1] = max(res.back()[1], each[1]);
        }
    }
    return res;
}

int main(void)
{
    vector<vector<int>> input{
        {1,3},
        {2,6},
        {8,10},
        {15,18}
    };
    /*vector<vector<int>> input{
        {1,4},
        {4,5},
    };*/
    vector<vector<int>> output = merge(input);

    for (auto each : output) {
        cout << "[" << each[0] << ", " << each[1] << "]";
    }
	return 0;
}